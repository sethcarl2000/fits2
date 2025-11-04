# fits2

> [!NOTE]
> I couldn't get ROOT to work in the Jupyter notebook, so i made a root-macro ```fit_histo.C``` which does the fitting & plotting. I have saved an output of this script under ```fit_info.txt```, and I saved an example output plot under ```fit_result.pdf```


Fitting examples:

  * User_fit.ipynb(C) : starter code and example for fitting an arbitrary function with ROOT
  * data1.root : created with User_fit.C 
  * Scipy_fit.ipynb : starter code and example for fitting an arbitrary function with scipy.optimize
  * datadist.root : contains a hisogram "h" that should be fit using a function of your own choosing

1) Examine the notebook User_fit.ipynb.  Your in-class project will be to improve the fit to converge on the falling distribution with two "bumps".
2) You can run the C++ version of the example using either:<br>
```
$ root User_fit.C
or
root[0] .L User_fit.C
root[1] User_fit()
```
See the code for additional details.

3) Look at Scipy_fit.ipynb for a brief example of doing a fiot with Scipy
4) Your main project is to complete the last section of either User_fit.ipynb (recommended) or Scipy_fit.ipynb.  Here you will develop your own model to fit a data set that is provided.  Details for your analysis are given in the notebook.
