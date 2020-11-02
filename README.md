# Arctic Subpolar gyre sTate Estimate (ASTE)

[![Binder](http://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/crios-ut/aste/master?filepath=aste_llcreader_example.ipynb)
[![DOI](https://zenodo.org/badge/214038615.svg)](https://zenodo.org/badge/latestdoi/214038615)

Nguyen, An T., Ocaña, V., Pillar, H., Bigdeli, A., Smith, T. A., & Heimbach, P. (2021). The Arctic Subpolar gyre sTate Estimate: a data-constrained and dynamically consistent ocean-sea ice estimate for 2002–2017. Submitted to Journal of Advances in Modeling Earth Systems.


## Analyzing ASTE Output

### Python Users

Click on the Binder button above to see how to access ASTE Release 1 output,
using [xmitgcm](https://xmitgcm.readthedocs.io/en/latest/)
to create an
[xarray](http://xarray.pydata.org/en/stable/)
Dataset on the fly with [dask](https://dask.org/).
Calculations and visualizations are enabled by
[xgcm](https://xgcm.readthedocs.io/en/latest/)
and [ecco_v4_py](https://ecco-v4-python-tutorial.readthedocs.io/index.html).

### MATLAB Users

Please see [this web portal](https://web.corral.tacc.utexas.edu/OceanProjects/ASTE/Release1) to download the output.
Note that the output is distributed differently for matlab and python.
The python demo above shows the horizontal ASTE grid as made up of 6 "tiles",
where each tile is 270x270 grid points.
The netcdf files listed below are split such that each file contains a single
90x90 tile, and 30 tiles make up the whole grid, see e.g.
[here](https://web.corral.tacc.utexas.edu/OceanProjects/ASTE/Release1/ASTE_tiles.png).
- [Release1/nctiles_monthly](https://web.corral.tacc.utexas.edu/OceanProjects/ASTE/Release1/nctiles_monthly): ASTE Release 1 monthly mean fields output as tiled NetCDF files
- [Release1/nctiles_monthly_snapshots](https://web.corral.tacc.utexas.edu/OceanProjects/ASTE/Release1/nctiles_monthly_snapshots): ASTE Release 1 snapshots of monthly fields output as tiled NetCDF files
- [Release1/nctiles_climatology](https://web.corral.tacc.utexas.edu/OceanProjects/ASTE/Release1/nctiles_climatology): ASTE Release 1 climatology fields output as tiled NetCDF files
- [Release1/nctiles_grid](https://web.corral.tacc.utexas.edu/OceanProjects/ASTE/Release1/nctiles_grid): ASTE Release 1 grid fields as tiled NetCDF files
- [matlab_tools](https://web.corral.tacc.utexas.edu/OceanProjects/ASTE/Release1/matlab_tools/): MATLAB files for reading the tiled NetCDF files
