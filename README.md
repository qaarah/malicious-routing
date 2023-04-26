 
----------------------------
 
 
# Malicious Routing of a Trojan Circuit in FPGAs
This project contains source code of the modified Tools for the insertion and activation of a Trojan circuit in FPGAs. 
## **Requirements**
1. Yosys (version 0.9 release recommended)
1. SystemC version 2.3.1a
1. Arachne-pnr 
1. iceprog
1. icestorm (recommended)
1. iCE40HX-1k device iCEstick Evaluation Kit

## **List of directories**
- _**Diff**_: contains the difference of ASCI, blif and log file of original and Trojan inserted circuits.
- _**Examples**_: contains original benchmark source code directories of AES and and AND gate circuits.
- _**Scripts**_: contains the bash script of complete flow.
- _**Tools**_: contains the modified source code of the tools _*Arachne-pnr_* and _*iceprog_* .

## **Steps to run the script:**
1. Run the script 'and_Example_flow.sh' after setting up the options inside. The options are:
    1. Original tools
    1. Modified Tools
    1. Examples

The above scripts will then synthesize the given example benchmark files, insert Trojans if modified tools are used, and generate the Bitstream. 
After the Bitstream has been generated using the original or the modified tools, the iCE40HX-1k FPGA can be configured.

## **Citation:**
Please cite the paper as following:

Q. A. Ahmed, T. Wiersema and M. Platzner, "Malicious Routing: Circumventing Bitstream-level Verification for FPGAs," 2021 Design, Automation & Test in Europe Conference & Exhibition (DATE), 2021, pp. 1490-1495, doi: 10.23919/DATE51398.2021.9474026.
 

----
