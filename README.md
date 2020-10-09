# electrodep

Codes, data and analysis of results for the paper: "Design rules for liquid crystalline electrolytes for enabling dendrite-free lithium metal batteries" [Proc. Natl. Acad. Sci. U.S.A.](https://www.pnas.org/content/early/2020/10/07/2008841117)

The preprint is available from [arXiv:1907.04441](https://arxiv.org/abs/1907.04441)

`electrodep` is a MOOSE-based application to simulate metal electrodeposition using a phase-field model

The standard input and analysis files for reproducing the results are in `/test/tests/paper-files` folder.

### Liquid Crystalline Electrolyte Battery Phase-Field Simulation Data
The data that support the findings of the paper are also also available in the form of `.mat` and `.m` files in the directory `LC-data`.

## How to run

Install MOOSE framework from [https://www.mooseframework.org/getting_started/index.html](https://www.mooseframework.org/getting_started/index.html)
```
git clone https://github.com/ahzeeshan/electrodep.git
cd electrodep
make -j4
cp electrodep-opt test/tests/paper-files/
cd test/tests/paper-files/
mpirun -np 24 ./electrodep-opt -i liqcrys_Kp_p_1.0E+01_c_1.0E+00_paper.i
```

## How to cite

```
@article{ahmad2020dendrite,
  title={Design rules for liquid crystalline electrolytes for enabling dendrite-free lithium metal batteries},
  elocation-id = {202008841},
  author={Ahmad, Zeeshan and Hong, Zijian and Viswanathan, Venkatasubramanian},
  doi = {10.1073/pnas.2008841117},
	publisher = {National Academy of Sciences},
  journal={Proceedings of the National Academy of Sciences},
  issn = {0027-8424},
	URL = {https://www.pnas.org/content/early/2020/10/07/2008841117},
	eprint = {https://www.pnas.org/content/early/2020/10/07/2008841117.full.pdf},
  year={2020}
}
```
