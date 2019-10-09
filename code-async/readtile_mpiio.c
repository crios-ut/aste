//
// MPI IO for MITgcm
// Author: Bron Nelson, February 28, 2019
// Adapted to ASTE: Tim Smith, October 9, 2019
//

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>

#include "SIZE.h"


// lat-lon-cap decomposition has 13 square facets
// facetElements1D is typically 1080, or 2160, or 4320



//////////////////////////////////////////////////////////////
// These values filled in during "initSizesAndTypes()"
MPI_Datatype  fieldElementalTypeMPI;
size_t  sizeofFieldElementalType;

long int tileSizeX;     // = sNx = 15
long int tileSizeY;     // = sNy = 15
long int xGhosts;       // = OLx = 4
long int yGhosts;       // = OLy = 4

long int facetElements1D; // = sFacet = 90
long int facetBytes2D;  // = sFacet * sFacet * datatype , datatype = MPI_DOUBLE

/* Global
 * ------
long int facetTilesInX; // = sFacet / sNx  = 90/15
long int facetTilesInY; // = sFacet / sNy  = 90/15
long int tilesPerFacet; // = facetTilesX * facetTilesY
*/
/* ASTE
 * ---- */
long int facetTilesInX_1;
long int facetTilesInY_1;
long int tilesPerFacet_1;

long int facetTilesInX_2;
long int facetTilesInY_2;
long int tilesPerFacet_2;

long int facetTilesInX_3;
long int facetTilesInY_3;
long int tilesPerFacet_3;

long int facetTilesInX_4;
long int facetTilesInY_4;
long int tilesPerFacet_4;

long int facetTilesInX_5;
long int facetTilesInY_5;
long int tilesPerFacet_5;


/* rather than 13 * 2d number of bytes, we want to compute the second 
 * dimension of the binary input field, which we set as fieldZlevelSize, then
 * multiply by datatype size to get fieldZlevelSizeInBytes
 *
 * we change the computation to use fieldZlevelSize as follows:
 *
 * Global
 * ------
 * fieldZlevelSize = 13 * facetElements1D * facetElements1D
 *                   13 *     90          *      90
 *
 * ASTE
 * ----
 * fieldZlevelSize = ( facetElement1D + 2*ncut1 + ncut2 ) * facetElement1D
 *                   (     90           2*150   +  60   ) *     90
 *                       
 * For both
 * --------
 * fieldZlevelSizeInBytes = fieldZlevelSize * sizeofFieldElementalType
 *
 */

long int fieldZlevelSize;
long int fieldZlevelSizeInBytes;

long int tileZlevelSizeInBytes;     // sNx * sNy * sizeofFieldElementalType = 15*15*XX
long int ghostedTileZlevelSizeInBytes; // (sNx + 2* OLx)*(sNy + 2* OLy) * sizeofFieldElementalType

// Global
// ------
// The first 7 facets all use the same style of layout; here we call
// them "section1".  The last 6 facets share a layout style, but this
// style is different than section1.  Here, we call them "section2".
//
// ASTE
// ----
// Define 3 datatypes for the 5 facets. 
// Note facet 2 is dropped for ASTE but numbering stays the same...
//
// section1 datatype works for tile on facet 1 & 3
// section2 datatype works for tile on facet 4
// section3 datatype works for tile on facet 5
MPI_Datatype  section1_ioShape2D, section2_ioShape2D, section3_ioShape2D;
MPI_Datatype  tileShape2D, ghostedTileShape2D;
MPI_Info  ioHints;
//////////////////////////////////////////////////////////////



int
getSizeOfMPIType(MPI_Datatype mpi_type)
{
    switch(mpi_type) {

      case MPI_INT:  
        return 4;
      case MPI_FLOAT:  
        return 4;
      case MPI_REAL4:
        return 4;
      break;

      case MPI_LONG_INT:  
        return 8;
      case MPI_DOUBLE:  
        return 8;
      case MPI_REAL8:
        return 8;
      break;

      default:
        assert(("unexpected mpi elemental type", 0));
      break;

    }
    return -1;
}



void
createMPItypes(void)
{
    /* --- ASTE ---*/
    // Create a type with the "shape" of a section1, 2D tile
    // contains  tileSizeY blocks, where each block has tileSizeX pieces of 
    // information (i.e. piece of info = data at a grid cell).
    // To go from one address to the next, skip over facetElements1D "pieces of info"

    // section1: facets 1 & 3
    MPI_Type_vector(tileSizeY, tileSizeX, facetElements1D,
                    fieldElementalTypeMPI, &section1_ioShape2D);
    MPI_Type_commit(&section1_ioShape2D);

    // section2: facet 4
    MPI_Type_vector(tileSizeY, tileSizeX, ncut2,
                    fieldElementalTypeMPI, &section2_ioShape2D);
    MPI_Type_commit(&section2_ioShape2D);

    // section3: facet 5
    MPI_Type_vector(tileSizeY, tileSizeX, ncut1,
                    fieldElementalTypeMPI, &section3_ioShape2D);
    MPI_Type_commit(&section3_ioShape2D);


    // Create a type that describes a 2D tile in memory
    MPI_Type_vector(tileSizeY, tileSizeX, tileSizeX,
                    fieldElementalTypeMPI, &tileShape2D);
    MPI_Type_commit(&tileShape2D);

    // Create a type that describes a 2D tile in memory with ghost-cells.
    int fullDims[2] = {tileSizeX + 2*xGhosts, tileSizeY + 2*yGhosts};
    int tileDims[2] = {tileSizeX, tileSizeY};
    int startElements[2] = {xGhosts, yGhosts};
    MPI_Type_create_subarray(2, fullDims, tileDims, startElements,
              MPI_ORDER_FORTRAN, fieldElementalTypeMPI, &ghostedTileShape2D);
    MPI_Type_commit(&ghostedTileShape2D);


    // Set up some possible hints
    MPI_Info_create(&ioHints);
    MPI_Info_set(ioHints, "collective_buffering", "true");
    char blockSize[64];
    sprintf(blockSize, "%ld", (((long)facetElements1D * 3) *
            tileSizeY) * sizeofFieldElementalType);
    MPI_Info_set(ioHints, "cb_block_size", blockSize);
}



void
initSizesAndTypes(void)
{
    /////////////////////////////////////////////
    // Fundamental values
    fieldElementalTypeMPI = MPI_DOUBLE;
    facetElements1D = sFacet;
    tileSizeX = sNx;
    tileSizeY = sNy;
    xGhosts = OLx;
    yGhosts = OLy;
    /////////////////////////////////////////////

    // Derived values
    sizeofFieldElementalType = getSizeOfMPIType(fieldElementalTypeMPI);
    long int facetElements2D = ((facetElements1D) * (facetElements1D));
    facetBytes2D  = (facetElements2D * sizeofFieldElementalType);

    // ASTE
    fieldZlevelSize = ( facetElements1D + 2*ncut1 + ncut2 ) * facetElements1D;
    fieldZlevelSizeInBytes = fieldZlevelSize * sizeofFieldElementalType;

    // Here we need to define number of tiles per facet
    // in ASTE there are 5 different possibilities (the Pacific is zero...)

    // 1. East North Atlantic
    facetTilesInX_1 = ((facetElements1D)/(tileSizeX));
    facetTilesInY_1 = ((ncut1)/(tileSizeY));
    tilesPerFacet_1 = ((facetTilesInX_1)*(facetTilesInY_1));

    // 2. Pacific: blank
    facetTilesInX_2 = 0;
    facetTilesInY_2 = 0;
    tilesPerFacet_2 = 0;

    // 3. Arctic !!!
    //      same specs as for standard global setup
    facetTilesInX_3 = ((facetElements1D)/(tileSizeX));
    facetTilesInY_3 = ((facetElements1D)/(tileSizeY));
    tilesPerFacet_3 = ((facetTilesInX_3)*(facetTilesInY_3));

    // 4. North Pacific (Bering Strait)
    facetTilesInX_4 = ((ncut2)/(tileSizeX));
    facetTilesInY_4 = ((facetElements1D)/(tileSizeY));
    tilesPerFacet_4 = ((facetTilesInX_4)*(facetTilesInY_4));

    // 5. West North Atlantic
    facetTilesInX_5 = ((ncut1)/(tileSizeX));
    facetTilesInY_5 = ((facetElements1D)/(tileSizeY));
    tilesPerFacet_5 = ((facetTilesInX_5)*(facetTilesInY_5));

    tileZlevelSizeInBytes = tileSizeX * tileSizeY * sizeofFieldElementalType;
    ghostedTileZlevelSizeInBytes = (tileSizeX + 2*xGhosts) *
                                   (tileSizeY + 2*yGhosts) *
                                   sizeofFieldElementalType;

    // Create the specialized type definitions
    createMPItypes();
}



void
tileIO(
  MPI_Comm comm,
  char *filename,
  MPI_Offset tileOffsetInFile,
  MPI_Datatype tileLayoutInFile,
  void *tileBuf,
  MPI_Datatype tileLayoutInMemory,
  int  writeFlag)
{
    int fileFlags;
    MPI_File fh;
    int (*new_MPI_IO)();

    int res,count;
    MPI_Status status;

    if (writeFlag) {
        fileFlags = MPI_MODE_WRONLY | MPI_MODE_CREATE;
        new_MPI_IO = MPI_File_write_all;
    } else {
        fileFlags = MPI_MODE_RDONLY;
        new_MPI_IO = MPI_File_read_all;
    }

    //printf("filename is %s\n",filename);

    MPI_File_open(comm, filename, fileFlags, ioHints, &fh);
    MPI_File_set_view(fh, tileOffsetInFile, fieldElementalTypeMPI,
                      tileLayoutInFile, "native", ioHints);


    // MPI_IO(fh, tileBuf, 1, tileLayoutInMemory, MPI_STATUS_IGNORE);
    res = new_MPI_IO(fh, tileBuf, 1, tileLayoutInMemory, &status);
    
    MPI_Get_count(&status,tileLayoutInFile,&count);

    //fprintf(stderr,"MPI: %d %d\n",res,count);

    MPI_File_close(&fh);
}






/* --- ASTE --- */
/* Determine which section datatype to use based on 
 * the tileID, which is determined from pkg/exch2 as:
 *
 *  tileID = tN = w2_myTileList(bi,bj)
 *
 * The functions below determine if in section1 or 2, being in section3 
 * is "otherwise"
 */

// N.B.: tileID is 1-based, not 0-based
inline int
isInSection1(int tileID)
{ 
    return (tileID <= (tilesPerFacet_1 + tilesPerFacet_2 + tilesPerFacet_3)); 
}
inline int
isInSection2(int tileID)
{ 
    return (tileID > (tilesPerFacet_1 + tilesPerFacet_2 + tilesPerFacet_3)
            &&
            tileID <= (tilesPerFacet_1 + tilesPerFacet_2 + tilesPerFacet_3 + tilesPerFacet_4)
           ); 
}


// N.B.: tileID is 1-based, not 0-based
long int
tileOffsetInField(int tileID)
{

    if (isInSection1(tileID)){
        return ((tileID-=1),
               (((long)tileID / facetTilesInX_1)*tileZlevelSizeInBytes * facetTilesInX_1) +
               (((long)tileID % facetTilesInX_1)*tileSizeX * sizeofFieldElementalType));
    } else if (isInSection2(tileID)){
        return ((tileID -= 1 + (tilesPerFacet_1 + tilesPerFacet_2 + tilesPerFacet_3)),
               (((long)tileID / facetTilesInX_4)*tileZlevelSizeInBytes * facetTilesInX_4) +
               (((long)tileID % facetTilesInX_4)*tileSizeX * sizeofFieldElementalType));
    } else {
        return ((tileID -= 1 + (tilesPerFacet_1 + tilesPerFacet_2 + tilesPerFacet_3 + tilesPerFacet_4)),
               (((long)tileID / facetTilesInX_5)*tileZlevelSizeInBytes * facetTilesInX_5) +
               (((long)tileID % facetTilesInX_5)*tileSizeX * sizeofFieldElementalType));
    }
}



void
readField(
  MPI_Comm  appComm,
  char *filename,
  MPI_Offset fieldOffsetInFile,
  void *tileBuf,
  int tileID,
  int zLevels)
{
    int writeFlag = 0;
    MPI_Comm  sectionComm = MPI_COMM_NULL;
    MPI_Datatype  section1_ioShape, section2_ioShape, section3_ioShape;
    MPI_Datatype  inMemoryShape, tileShape, ghostedTileShape;

    int inSection1 = isInSection1(tileID);
    int inSection2 = isInSection2(tileID);
    MPI_Offset tileOffsetInFile = fieldOffsetInFile + tileOffsetInField(tileID);


    // Create a type with the "shape" of a tile in memory,
    // with the given number of z-levels.
    MPI_Type_hvector(zLevels, 1, tileZlevelSizeInBytes,
                     tileShape2D, &tileShape);
    MPI_Type_commit(&tileShape);

    // Create a type with the "shape" of a tile in memory,
    // with ghost-cells, with the given number of z-levels.
    MPI_Type_hvector(zLevels, 1, ghostedTileZlevelSizeInBytes,
                     ghostedTileShape2D, &ghostedTileShape);
    MPI_Type_commit(&ghostedTileShape);


    // choose the i/o type
    inMemoryShape = tileShape;


    // Split between section1 tiles and section2 tiles.
    // If a rank has been assigned multiple tiles, it is possible that
    // some of those tiles are in section1, and some are in section2.
    // So we have to dynamically do the comm_split each time because we
    // cannot absolutely guarentee that each rank will always be on the
    // same side of the split every time.
    MPI_Comm_split(appComm, inSection1, 0, &sectionComm);

    memset(tileBuf,-1,tileZlevelSizeInBytes*zLevels);

    if (inSection1) {

        // Create a type with the "shape" of a section1 tile
        // in the file, with the given number of z-levels.
        MPI_Type_hvector(zLevels, 1, fieldZlevelSizeInBytes,
                         section1_ioShape2D, &section1_ioShape);
        MPI_Type_commit(&section1_ioShape);

	//printf("section 1: %d -> %ld (%ld + %ld)\n",tileID,tileOffsetInFile,fieldOffsetInFile,tileOffsetInField(tileID));

        // Do the i/o
        tileIO(sectionComm, filename, tileOffsetInFile, section1_ioShape,
               tileBuf, inMemoryShape, writeFlag);
        // I believe (?) this is needed to ensure consistency when writting
        if (writeFlag)  MPI_Barrier(appComm);

        MPI_Type_free(&section1_ioShape);

    } else if (inSection2) {

        // Create a type with the "shape" of a section2 tile
        // in the file, with the given number of z-levels.
        MPI_Type_hvector(zLevels, 1, fieldZlevelSizeInBytes,
                         section2_ioShape2D, &section2_ioShape);
        MPI_Type_commit(&section2_ioShape);

	//printf("section 2: %d -> %ld (%ld + %ld)\n",tileID,tileOffsetInFile,fieldOffsetInFile,tileOffsetInField(tileID));

        // Do the i/o
        // I believe (?) this is needed to ensure consistency when writting
        if (writeFlag)  MPI_Barrier(appComm);
        tileIO(sectionComm, filename, tileOffsetInFile, section2_ioShape,
               tileBuf, inMemoryShape, writeFlag);

        MPI_Type_free(&section2_ioShape);

    } else {

        // Create a type with the "shape" of a section3 tile
        // in the file, with the given number of z-levels.
        MPI_Type_hvector(zLevels, 1, fieldZlevelSizeInBytes,
                         section3_ioShape2D, &section3_ioShape);
        MPI_Type_commit(&section3_ioShape);

	//printf("section 2: %d -> %ld (%ld + %ld)\n",tileID,tileOffsetInFile,fieldOffsetInFile,tileOffsetInField(tileID));

        // Do the i/o
        // I believe (?) this is needed to ensure consistency when writting
        if (writeFlag)  MPI_Barrier(appComm);
        tileIO(sectionComm, filename, tileOffsetInFile, section3_ioShape,
               tileBuf, inMemoryShape, writeFlag);

        MPI_Type_free(&section3_ioShape);
    }

    /*
    if (tileID==315){
      int i;
      printf("field offset: %ld   ",fieldOffsetInFile);
      printf("tile offset: %ld    ",tileOffsetInField(tileID));
      printf("zlevels: %d     ",zLevels);
      for (i=0;i<10;++i)
	printf("%f ",((double*)tileBuf)[i]);
      printf("\n");
    }
    */

    // Clean up
    MPI_Type_free(&tileShape);
    MPI_Type_free(&ghostedTileShape);
    MPI_Comm_free(&sectionComm);
}



// Fortran interface
// This uses the "usual" method for passing Fortran strings:
// the string length is passed, by value, as an extra "hidden" argument
// after the end of the normal argument list.  So for example, this
// routine would be invoked on the Fortran side like this:
//     call readField(comm, filename, offset, tilebuf, tileid, zlevels)
// This method of passing FOrtran strings is NOT defined by the Fortran
// standard, but it is the method of choice for many compilers, including
// gcc (GNU/Linux), and icc (Intel).
//
// PLEASE NOTE that the "offset" field is of type "MPI_Offset", which
// is synonymous with the Fortran type "integer(kind=MPI_OFFSET_KIND)".
// This will typically be integer*8.  But in particular it is almost
// certainly NOT of type "default integer", which means in particular
// that you CANNOT simply pass a constant (e.g. "0") as the argument,
// since that type will be of the wrong size.
void
readfield_(
  MPI_Fint  *fortranAppComm,
  char *fortranFilename,
  int  *fieldOffsetInFileInPencils,
  void *tileBuf,
  int *tileID,
  int *zLevels,
  int filenameLength)
{
    int i;
    char namebuf[filenameLength+1];
    char *filename = namebuf;

//fprintf (stderr, "In NEW readfield_\n");

    MPI_Offset fieldOffsetInFile = *fieldOffsetInFileInPencils * tileSizeX * sizeofFieldElementalType;

    // Translate the MPI communicator from a Fortran-style handle
    // into a C-style handle.
    MPI_Comm appComm = MPI_Comm_f2c(*fortranAppComm);

    // Translate the Fortran-style string into a C-style string
    //memset(filename, ' ', filenameLength));
    strncpy(filename, fortranFilename, filenameLength);
    for (i = filenameLength;  (i > 0) && (' ' == filename[i-1]);  --i) ;
    filename[i] = '\0';
    //while(' ' == *filename) ++filename;
    assert(strlen(filename) > 0);

    //fprintf(stderr,"%d ::%s:: %d %ld  \n",appComm,filename,filenameLength,fieldOffsetInFile);

    // Make the translated call
    readField(appComm, filename, fieldOffsetInFile, tileBuf, *tileID, *zLevels);
}



// For testing
void initsizesandtypes_(void) {initSizesAndTypes();}





/////////////////////////////////////////////////////////////
// Test case
#if 0

#define FILENAME  "./dataFile"
long int fieldOffsetInFile = 0;

void
doIO(MPI_Comm appComm)
{
    int sizeZ = 3;
    int tile1[sizeZ][tileSizeY][tileSizeX];
    int tile2[sizeZ][tileSizeY][tileSizeX];
    int ghostedTile[sizeZ][tileSizeY + 2*yGhosts][tileSizeX + 2*xGhosts];
    int tileID;
    int i,j,k;

    int  appCommSize, appCommRank;
    MPI_Comm_size(appComm, &appCommSize);
    MPI_Comm_rank(appComm, &appCommRank);

    assert((facetTilesInX * tileSizeX) == facetElements1D);
    assert((facetTilesInY * tileSizeY) == facetElements1D);

    // Ignore the dry tiles ("holes") for the moment
    if (facetTilesInX * facetTilesInY * 13 != appCommSize) {
        if (0 == appCommRank) {
            printf("Unexpected number of ranks: is %d, expected %ld\n",
                   appCommSize, facetTilesInX * facetTilesInY * 13);
        }
    }
    tileID = appCommRank + 1;

#if 0
    // Fill tile1 with distinguished values
    for (k = 0;  k < sizeZ;  ++k) {
        for (j = 0;  j < (tileSizeY + 2*yGhosts);  ++j) {
            for (i = 0;  i < (tileSizeX + 2*xGhosts);  ++i) {
                ghostedTile[k][j][i] = -appCommRank;
            }
        }
    }
    for (k = 0;  k < sizeZ;  ++k) {
        for (j = 0;  j < tileSizeY;  ++j) {
            for (i = 0;  i < tileSizeX;  ++i) {
                tile1[k][j][i] = appCommRank;
                ghostedTile[k][j+yGhosts][i+xGhosts] = appCommRank;
            }
        }
    }
#endif



if (0 == appCommRank) system("/bin/echo -n 'begin io: ' ; date ");
    readField(appComm, FILENAME, 0, ghostedTile, tileID, sizeZ);
if (0 == appCommRank) system("/bin/echo -n 'half: ' ; date ");
    readField(appComm, FILENAME, sizeZ*fieldZlevelSizeInBytes,
              ghostedTile, tileID, sizeZ);

#if 1
    for (k = 0;  k < sizeZ;  ++k) {
        for (j = 0;  j < tileSizeY;  ++j) {
            for (i = 0;  i < tileSizeX;  ++i) {
                int value = ghostedTile[k][j+yGhosts][i+xGhosts];
                if (value != appCommRank) {
                    printf("Fail: %d %d %d:  %d %d\n", k,j,i, value, appCommRank);
                    exit(1);
                }
            }
        }
    }
    if (0 == appCommRank) printf("Verification complete\n");
#endif

MPI_Barrier(appComm);
if (0 == appCommRank) system("/bin/echo -n 'finish: ' ; date ");

    MPI_Finalize();
}



int
main(int argc, char *argv[])
{
    MPI_Comm  appComm = MPI_COMM_NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_dup(MPI_COMM_WORLD, &appComm);

    initSizesAndTypes();
    doIO(appComm);

    MPI_Finalize();
    return 0;
}
#endif

