# BFIDE
Simple Linux IDE for editing and running BrainFuck code
## Dependencies:
- terminator
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
But anyway, if you face with some troubles, you can always open a new issue on issues tab.  
## Setting up BFIDE  
After installation, you have specify path to brainfuck compiler.
By default, compiler path is set to '/usr/bin/brainfuck', but you can change it in settings.  
## Credits:  
- [CQtDeployer](https://github.com/QuasarApp/CQtDeployer)
- [Code Editor Class](https://code.qt.io/cgit/qt/qtbase.git/tree/examples/widgets/widgets/codeeditor?h=5.15)
- [gitignore.io](https://github.com/toptal/gitignore.io)
