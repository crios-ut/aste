# Arctic Subpolar gyre sTate Estimate (ASTE).

[![Binder](http://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/timothyas/aste-release1/main?filepath=aste_llcreader_example.ipynb)

## Analyzing ASTE Output

Click on the Binder button above to see how to access ASTE Release 1 output,
using [xmitgcm](https://xmitgcm.readthedocs.io/en/latest/).
to create an
[xarray](http://xarray.pydata.org/en/stable/)
Dataset on the fly with [dask](https://dask.org/).
Calculations and visualizations are enabled by
[xgcm](https://xgcm.readthedocs.io/en/latest/)
and [ecco_v4_py](https://ecco-v4-python-tutorial.readthedocs.io/index.html).

## Instructions to re-run ASTE

Specific compile and runtime options coming soon!

Checkout the [MITgcm](https://github.com/MITgcm/MITgcm):

```
git clone https://github.com/MITgcm/MITgcm.git
```
or
```
git clone git@github.com:MITgcm/MITgcm.git
```

and get checkout 65q as follows:

``` 
cd MITgcm
git checkout checkpoint65q
```


And see the [MITgcm documentation](https://mitgcm.readthedocs.io/en/latest/index.html) for instructions on 
[building](https://mitgcm.readthedocs.io/en/latest/getting_started/getting_started.html#building-the-model)
and 
[running](https://mitgcm.readthedocs.io/en/latest/getting_started/getting_started.html#running-the-model)
the model.



