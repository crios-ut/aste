C $Header: /u/gcmpack/MITgcm/pkg/ecco/cost_gencost_boxmean.F,v 1.9 2015/11/17 16:05:28 gforget Exp $
C $Name: checkpoint65q $

#include "ECCO_OPTIONS.h"

      subroutine cost_gencost_boxmean(mythid)

c     ==================================================================
c     SUBROUTINE cost_gencost_boxmean
c     ==================================================================
c
c     o Evaluate cost function contributions of box mean THETA.
c
c     ==================================================================
c     SUBROUTINE cost_gencost_boxmean
c     ==================================================================

      implicit none

c     == global variables ==

#include "EEPARAMS.h"
#include "SIZE.h"
#include "PARAMS.h"
#include "GRID.h"
#ifdef ALLOW_CAL
# include "cal.h"
#endif
#ifdef ALLOW_ECCO
# include "ecco.h"
#endif

c     == routine arguments ==
      integer mythid

#ifdef ALLOW_GENCOST_CONTRIBUTION

c     == local variables ==

      integer kgen
      _RL mybar(1-olx:snx+olx,1-oly:sny+oly,nsx,nsy)

      _RL mySumTile(nSx,nSy),myVolTile(nSx,nSy)
      _RL mySumGlo,myVolGlo,myMeanGlo

      _RL tmpSumTile(nSx,nSy),tmpVolTile(nSx,nSy)
      _RL tmpSumGlo,tmpVolGlo,tmpMeanGlo

      _RL gencost_mskTemporal
      _RL Nrecsum
      _RL tmpfac(30000)

      integer bi,bj
      integer i,j,k
      integer nrecloc,irec,il
      character*(80) myfname
      _RL mydummy
      logical doglobalread
      logical ladinit
      character*(MAX_LEN_MBUF) msgbuf

c     == external functions ==

      integer  ilnblnk
      external ilnblnk

      LOGICAL  MASTER_CPU_THREAD
      EXTERNAL MASTER_CPU_THREAD

c     == end of interface ==

c-- detect the relevant gencost indices
      kgen=0
      do kgen=1,NGENCOST
        if( (gencost_flag(kgen).EQ.-3).and.(using_gencost(kgen)) ) then

c ========

c set bar field params
      doglobalread = .false.
      ladinit      = .false.
      mydummy=gencost_dummy(kgen)
      il = ilnblnk( gencost_barfile(kgen) )
      write(myfname(1:80),'(2a,i10.10)')
     &    gencost_barfile(kgen)(1:il),'.',eccoiter

c initialize various things to 0
      DO bj=myByLo(myThid),myByHi(myThid)
       DO bi=myBxLo(myThid),myBxHi(myThid)
          mySumTile(bi,bj)=0. _d 0
          myVolTile(bi,bj)=0. _d 0
          mySumGlo=0. _d 0
          myVolGlo=0. _d 0
       ENDDO
      ENDDO

      nrecloc=gencost_nrec(kgen)

Catn
Cdo time average (+1) or take last month (-1)
      Nrecsum = 0.
      do irec = 1, 30000
        tmpfac(irec) = 0.
      enddo
      do irec = 1,nrecloc
        if(gencost_preproc_i(1,kgen).lt.0) then
          tmpfac(irec)= 0.
          if (irec.eq.nrecloc) tmpfac(irec)= 1.
        else
          tmpfac(irec)= 1.
        endif
        Nrecsum=Nrecsum+tmpfac(irec)
      enddo

       WRITE(msgBuf,'(A,I3,A,1PE21.14)')
     &  'boxmean [-1(+1),1] :',gencost_preproc_i(1,kgen),' ',Nrecsum
      CALL PRINT_MESSAGE( msgBuf, standardMessageUnit,
     &    SQUEEZE_RIGHT, myThid )

c ========

c main loop where cost is computed and time series is displayed
      do irec = 1,nrecloc
c read bar field
#ifdef ALLOW_AUTODIFF
        call active_read_xy( myfname,mybar,irec,
     &                       doglobalread, ladinit,
     &                       eccoiter, mythid,
     &                       mydummy )
#else
        CALL READ_REC_XY_RL( myfname, mybar,
     &                       iRec, 1, myThid )
#endif

      DO bj=myByLo(myThid),myByHi(myThid)
       DO bi=myBxLo(myThid),myBxHi(myThid)
          tmpSumTile(bi,bj)=0. _d 0
          tmpSumGlo=0. _d 0
        enddo
      enddo

catn      gencost_mskTemporal = nrecloc
      gencost_mskTemporal = Nrecsum
      gencost_mskTemporal=1. _d 0 / gencost_mskTemporal

c compute cost

      if ( myProcId .EQ. 0 ) num_gencost(1,1,kgen) =
     &    num_gencost(1,1,kgen)+gencost_mskTemporal

      DO bj=myByLo(myThid),myByHi(myThid)
       DO bi=myBxLo(myThid),myBxHi(myThid)
            do j = 1,sNy
              do i =  1,sNx
c sum that is actually be used in cost function
      objf_gencost(bi,bj,kgen)=objf_gencost(bi,bj,kgen)
     &   +gencost_mskTemporal*mybar(i,j,bi,bj)
     &   *tmpfac(irec)

c sum for display of time series
      tmpSumTile(bi,bj)=tmpSumTile(bi,bj)
     &   +gencost_mskTemporal*mybar(i,j,bi,bj)
         if(abs(mybar(i,j,bi,bj)).gt.0. _d 0) then
         endif
              enddo
            enddo
        enddo
      enddo

c global sums for display of time series
c note that this is only for cosmetic, the fc should match
c the last record if gencost_preproc_i(1,kgen)=-1
      CALL GLOBAL_SUM_TILE_RL( tmpSumTile, tmpSumGlo, myThid )
       WRITE(msgBuf,'(A,I3,A,1PE21.14)')
     &     'boxmean/horflux :',irec,' ',tmpSumGlo
      CALL PRINT_MESSAGE( msgBuf, standardMessageUnit,
     &    SQUEEZE_RIGHT, myThid )

      enddo

c ========


c global sums for cost function
      CALL GLOBAL_SUM_TILE_RL( objf_gencost(1,1,kgen),
     &   mySumGlo, myThid )
      WRITE(msgBuf,'(A,1PE21.14)') 'boxmean/horflux fc :',mySumGlo
      CALL PRINT_MESSAGE( msgBuf, standardMessageUnit,
     &    SQUEEZE_RIGHT, myThid )

c ========

      endif !if ( (gencost_flag(kgen).EQ.-3).AND.
      enddo !do kgen=1,NGENCOST

#endif /* ALLOW_GENCOST_CONTRIBUTION */

      end
