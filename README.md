## P4Maya - P4GT Maya Plugin
P4Maya is a powerful Autodesk Maya plugin that seamlessly integrates with the Perforce version control system, allowing developers and designers to manage assets, collaborate across versions, and work in multiple languages—all without leaving the Maya environment.

## Prerequisites
- Maya SDK (From Autodesk Maya site)
- Jam build system (Download https://swarm.workshop.perforce.com/files/guest/perforce_software/jam/jam-2.6.1.zip)
- OpenSSL 3
- P4 source code (Download https://ftp.perforce.com/perforce/r25.1/bin.tools/p4source.tgz)
- Visual Studio (version matching the Autodesk Maya edition, per Autodesk's guidelines)

## Folder Structure 
Create a root folder named p4maya, then: 
	1. Create a folder p4-gt and clone the repository into it:
		git clone <repo-url> p4-gt
	2. Create a folder sdk\maya inside p4-gt 
		Download the Maya SDK from Autodesk and copy the include and lib folders into sdk\maya .
	3. Create a folder jam and unzip all Jam files into it.
	4. Create a folder openssl-3 and after building the openssl open source code , copy the include (header files) and lib folders creating structure as :
		openssl-3\include\openssl
		openssl-3\lib
		Open tools\setenv20xx.bat and 
		set SSLINCDIR=\path till\openssl-3\include 
		and SSLLIBDIR=\path till\openssl-3\lib
		(ensure openssl used must be compatible with visual studio version installed)
	5. Create a folder p4 and extract the contents of p4source.tgz into it.
	
Your folder structure should look like this -  
p4maya/
├── jam/						# Jam build system files
├── openssl-3/					# OpenSSL headers and libraries        
├── p4/                     	# P4API source and build scripts
├── p4-bin/                    	# Generated after building P4API 
│── p4-gt/           			# P4Maya source code         
│	├── libp4gt/				# Graphical components interfacing with P4API
│	├── maya/					# Maya plugin source
│	├── sdk/maya/				# Maya SDK files
│	├── tools/					# Build and utility scripts
│	├── Jamfile					# Jam build configuration
│	├── Jamrules				# Jam build rules
└── README.md 					# This file

## Build Instructions
To build the Maya plugin (.mll file):
	1. Open the Developer Command Prompt for Visual Studio. Navigate to the root folder: 
		cd p4maya
	2. Set the environment variables by choosing the appropriate setenv20xx.bat file based on your Visual Studio version:
		cd p4-gt\tools
		run setenv20xx.bat
	3. Go to the jam folder:
		cd p4maya/jam
		Ensure platform-specific configurations are uncommented in the Makefile, then run:
		run nmake -a
	4. Build the P4API:
		cd p4maya/p4
		run ..\p4-gt\tools\bldp4api.bat 
	(Note: It may skip few (~6) targets - this is expected and can be ignored.)
	5. Update version metadata:
		cd p4-gt\tools
		run UpdateVersion.bat
	6. Build the plugin:
		cd p4-gt
		run tools\bldmaya.bat
	7. Locate the output .mll file:
		For English: p4-bin\bin.ntx64\en\dyn
		For Japanese: p4-bin\bin.ntx64\ja\dyn

## Install
To install the plugin in Autodesk Maya:
	1. Copy the .mll file to Maya bin\Plug-ins directory:
	2. Open Maya and navigate to:
		Windows → Settings/Preferences → Plug-in Manager
	3. In the Plug-in Manager dialog:
		Search for P4GT-Maya-2026_x64.mll
		Check Loaded to activate the plugin
		Check Auto-Load to ensure it loads automatically on future launches
Once enabled, the P4 menu will appear in Maya’s main interface, allowing seamless Perforce integration.

## License
See `LICENSE.txt` for license information.