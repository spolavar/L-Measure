# L-Measure
[L-Measure Homepage](http://cng.gmu.edu:8080/Lm/)


[L-Measure manual](http://cng.gmu.edu:8080/Lm/help/index.htm)
## Overview
 The code is centered around Neuron object, which is a binary tree processed through a factory of methods to compute the statistics (avg, min, max, etc) on any given choice of measurements (length, angles, braches, etc). There are several function classes that are designed to compute the geometrical and topological metrics at different detail (segment, branch, and arbor) on the neuron object. Besides this core functionality, the supporting methods include of parsers for various reconstruction formats, custom printing formats (swc, vrml, etc), PCA-based orientation of the neuron for visualization, and so on.
 
## compiling L-Measure
1.	Use MinGW for GCC/G++ compiler for windows
    *	Setup L-Measure code on GCC/G++ compiler
    *	GCC/G++ for windows is provide by many vendors like CYGWIN, MinGW and others.
    *	This manual will help setting up L-Measure on MinGW - A C/C++ compiler suite which allows you to create Windows executables       without any dependency on external DLLs besides the normal MSVC runtimes.
    *	MinGW is a Windows port of the GNU compiler tools, like GCC, Make, Bash, etc, which run directly in Windows without any          emulation layer. By default it will compile to a native Win32 target, complete with .exe and .dll files.

2.	Generate L-Measure executable on Eclipse CDT for windows
    *	Download Eclipse IDE for C/C++ Developers from    http://www.eclipse.org/downloads/packages/eclipse-ide-cc-developers-includes-incubating-components/indigosr1
    *	Copy the zip into any desired location and unzip it.
    *	Download MinGW from http://sourceforge.net/projects/mingw/files/
    *	Install the MinGW with at least c and c++ options.
    *	Restart your machine.
    *	Go the eclipse folder, and click the eclipse icon. Choose a desired workspace location
    *	Once the IDE is launched, go to the workbench.
    *	Right click on the project explorer and go to New->Projects->C++ Project
    *	Select Project Type: Empty Project & Tool chains: MinGW GCC and select the project name “Lm” and click finish.
    *	A new empty project is created.
    *	Copy the code to your local drive from file server or where ever you have.
    *	Right click on Lm (project in the project explorer @ eclipse IDE) and navigate to Import->File System. Browse to the    folder having the code. Select all the files to be imported.
    *	Right click on the Lm (project in the project explorer @ eclipse IDE) and navigate to  Properties->C/C++ Build->Settings->Tool Settings -> MinGW c++ Linker -> Miscellaneous 
    *	In the Linker Flags add “-static-libgcc -static-libstdc++” and Apply and OK. 
    *	This  step is done in order to package the required GCC library dll with the executable.(Ref#3.2)
    *	In the same window (opened from Step#m), go to C/C++ Build -> Settings ->Tool Settings -> MinGW c++ Linker. Replace the Command: “g++” with “g++ -Wl,--stack,4194304”. Apply and Ok.
    *	This change helps us to increase the stack size to 4Mb from 1Mb.
    *	Right click and Build Project
    *	Remove error and rebuild in case of errors.
    *	Find the executable in the debug folder of the project.
3.	Setup Eclipse for Linux
      *	Download the desired eclipse package from http://www.eclipse.org/downloads/packages/eclipse-ide-cc-developers-includes-incubating-components/indigosr2
      *	Extract the downloaded  package
      *	Click on eclipse icon to open the IDE, and select workbench.
      *	Now make a new project File->New->C++ Project.
      *	Select Project Type: Empty Project & Toolchains: Linux GCC. Give a Project name : lmeasure and then click finish.
      *	Copy the code from the file server to the local machine.
      *	Right click on the project lmeasure and select import.
      *	Select File System and click Next.
      *	Select the directory, where the code is being copied.
      *	Select the files and click on finish.
      *	Go to Project->Build All to build the project.
4. GCC compilers L-Measure was tested on:
      * Centos(Cngfile Server) : ldd (GNU libc) 2.5
      * Fedora (Cng Server) : ldd (GNU libc) 2.10.2
      * Ubuntu (Standalone) : ldd (GNU libc) 2.10.1

