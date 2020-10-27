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

Please see [this web portal](https://web.corral.tacc.utexas.edu/OceanProjects/ASTE/) to download the output:
- `Release 1/`: ASTE Release 1 output as tiled NetCDF files
- `Analysis Tools/`: MATLAB files for reading the tiled NetCDF files
