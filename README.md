# electrodep

Codes, data and analysis of results for the paper: "Design rules for liquid crystalline electrolytes for enabling dendrite-free lithium metal batteries" [arXiv:1907.04441](https://arxiv.org/abs/1907.04441)

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
@article{ahmad2019dendrite,
  title={Design rules for liquid crystalline electrolytes for enabling dendrite-free lithium metal batteries},
  author={Ahmad, Zeeshan and Hong, Zijian and Viswanathan, Venkatasubramanian},
  journal={arXiv:1907.04441},
  year={2019}
}
```
