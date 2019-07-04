
combo - simple program generating all combinations of words you specified.
For example:
```sh
> combo  -n 2 -m password 123 login root
password
123
login
root
123password
loginpassword
rootpassword
password123
login123
root123
passwordlogin
123login
rootlogin
passwordroot
123root
loginroot
```
---
Installing:
```sh
git clone https://github.com/jienfak/combo
cd combo
make all
make clean
make install
```
---
Uninstalling:
```sh
cd combo
make uninstall
```
---
To get help read 'man combo'.
