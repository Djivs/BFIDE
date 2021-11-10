# BFIDE
Simple Linux IDE for editing and running BrainFuck code
## Requirements:
- Any terminal emulator (for example, xterm, terminator, konsole, ...)
- Any brainfuck compiler (for example, https://github.com/fabianishere/brainfuck)
## Instalation:
1. Go to releases page
2. Pick latest release
3. Download .zip file
4. Unzip this file
5. Make .run file executable and run it
6. Follow installer inctructions
## Troubleshooting during installation
Main part of all troubles, happening during installation, are linked with  
unsolved Qt dependencies. So, quick fix is to install these packages:
- build-dep 
- qt5-default
- build-dep
- qt5-default  

(Names can be different for different distributions)  
Or, if it didn't helped, you need to check out this [article](https://wiki.qt.io/Building_Qt_5_from_Git)  
If that didn't helped, open a new [issue](https://github.com/Djivs/BFIDE/issues)
## Setting up BFIDE  
After installation, go to settings and set up these parameters:
1. BrainFuck compiler path  
2. Terminal path  
3. [Optional] Shell command, which will be used to run code
## Credits:  
- [CQtDeployer](https://github.com/QuasarApp/CQtDeployer)
- [Code Editor Class](https://code.qt.io/cgit/qt/qtbase.git/tree/examples/widgets/widgets/codeeditor?h=5.15)
- [gitignore.io](https://github.com/toptal/gitignore.io)
