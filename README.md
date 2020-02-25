                                      _
                                     / /
       ____________      _   _      / /
      / ____/ ____/____/ / / /___  / /____________________
     / / __/ /   / ___/ / / / __ \/ _______Gaspar's cryptographic
    / /_/ / /___/ /  / /_/ / /_/ / /_      algorithm.
    \____/\____/ /   \__, / .___/\__/
              / /      / / / _____________________________________________
             /_/   ___/ / /    https://github.com/GasparVardanyan/gcrypt
                  /____/_/               _____   _________
                                         ___  \ / /__ '___
             51mP13, 53cUR3,                 \ V / |_ \
             p3rf0rM4nc3                      \ / |  _/
                                             / /  | |
<!--


## Features

* ### Simplity
* Simple algorithm, in 128 lines of code written in the C language.
* ### Security
* The overlapping probability of the randomly generated decryption hash and original hash is: ![<sup>1</sup>/<sub>256!</sub>](https://latex.codecogs.com/png.latex?\dpi{80}&space;\fn_phv&space;\frac{1}{256!}) <em>(1 / (256 factorial))</em>.
* The output file doesn't have a certain structure.
* ### Performance
* At 0.5 gbps encryption speed.
* The output file size is same as the input file size.

## Installation

### Installation on BlackArch Linux
`pacman -S gcrypt`

### Installation on Arch Linux
`mkdir GCrypt`<br>
`cd GCrypt`<br>
`curl -O https://raw.githubusercontent.com/BlackArch/blackarch/master/packages/gcrypt/PKGBUILD`<br>
`makepkg -si`<br>

### Installation on Linux
`git clone https://github.com/GasparVardanyan/GCrypt.git`<br>
`cd GCrypt`<br>
`sh $PWD/compile`<br>
`sudo cp $PWD/gcrypt /bin/`<br>
`sudo chmod +x /bin/gcrypt`<br>

<!--
### Windows executables
[Download GCrypt 32-bit](https://raw.githubusercontent.com/GasparVardanyan/GCrypt/master/windows/gcrypt32.exe)<br>
[Download GCrypt 64-bit](https://raw.githubusercontent.com/GasparVardanyan/GCrypt/master/windows/gcrypt64.exe)
-- >
-->
