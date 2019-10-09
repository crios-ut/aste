# aste
Contains the compile time and run time options for the
Arctic Subpolar gyre sTate Estimate (ASTE).

## Instructions

Checkout the [MITgcm](https://github.com/MITgcm/MITgcm):

```
git clone https://github.com/MITgcm/MITgcm.git
```
or
```
git clone https://github.com/MITgcm/MITgcm.git
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


## Contents
- code/ 
    - compile time options and changes from the main MITgcm branch
    - Note: this used to have the suffix `_BE2_dthetadr_sshv4mo_fix_logdkr` 
- code-async/
    - code enabling asynchronous output through the MITgcm 
- input/
    - run time options
    - Note: this used to have the suffix `_BE2_dthetadr_sshv4mo_fix_logdkr`
