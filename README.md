## P4Maya - P4GT Maya Plugin

**P4Maya** is a powerful Autodesk Maya plugin that seamlessly integrates with the Perforce version control system. It enables developers and designers to:
- Manage assets
- Collaborate across versions
- Work in multiple languages —all without leaving the Maya environment.

## Prerequisites
To build and use the plugin, ensure the following dependencies are installed:
- **Maya SDK**  Download from the Autodesk Maya site
- **Jam build system**  Download jam-2.6.1.zip
- **OpenSSL 3**
- **P4 source code**  Download p4source.tgz from www.perforce.com or the FTP site
- **Visual Studio**  Version matching the Autodesk Maya edition, per Autodesk's guidelines

## Folder Structure
Create a root folder named `p4maya`, then follow these steps:

1. **Clone the repository**  
   Create a folder named `p4-gt` and clone the repository into it:
   ```bash
   git clone <repo-url> p4-gt
  
2. **Add Maya SDK**  
   Inside p4-gt, create a folder sdk/maya.
   Download the Maya SDK from Autodesk and copy the include and lib folders into sdk/maya.
  
3. **Add Jam build system**  
   Create a folder named jam and unzip all Jam files into it.
  
4. **Add OpenSSL 3**  
   Create a folder named openssl-3. After building the OpenSSL source code, copy the following folders:
	```bash
	openssl-3\include\openssl
	openssl-3\lib
	```
 	Then, open tools/setenv20xx.bat and set the following environment variables:
	```bash
	set SSLINCDIR=\path\to\openssl-3\include
	set SSLLIBDIR=\path\to\openssl-3\lib
	```
 	(ensure openssl used must be compatible with visual studio version installed)
  
5. **Add P4 source code**  
   Create a folder named p4 and extract the contents of p4source.tgz into it.

Your folder structure should look like this -  
```bash
p4maya/
├── jam/                      # Jam build system files
├── openssl-3/                # OpenSSL headers and libraries
├── p4/                       # P4API source and build scripts
├── p4-bin/                   # Generated after building P4API
├── p4-gt/                    # P4Maya source code
│   ├── libp4gt/              # Graphical components interfacing with P4API
│   ├── maya/                 # Maya plugin source
│   ├── sdk/
│   │   └── maya/             # Maya SDK files
│   ├── tools/                # Build and utility scripts
│   ├── Jamfile               # Jam build configuration
│   ├── Jamrules              # Jam build rules
└── README.md                 # This file
```

## Build Instructions

To build the Maya plugin (`.mll` file), follow these steps:
1. **Open the Developer Command Prompt for Visual Studio**  
   Navigate to the root folder:
   ```bash
   cd p4maya
   ```

2. **Set the environment variables**  
   Choose the appropriate setenv20xx.bat file based on your Visual Studio version:
   ```bash
   cd p4-gt\tools
   setenv20xx.bat
   ```

3. Go to the jam folder and build it:
   ```bash
   cd p4maya/jam
   nmake -a
   ```

4. Build the P4API:
   ```bash
   cd p4maya/p4
   ..\p4-gt\tools\bldp4api.bat 
	```
   (Note: It may skip few (~6) targets - this is expected and can be ignored.)
	
 5. Update version metadata:
    ```bash
    cd p4-gt\tools
    UpdateVersion.bat
    ```
    
6. Build the plugin:
   ```bash
   cd p4-gt
   tools\bldmaya.bat
   ```
   
7. Locate the output .8li file:
   ```bash
   For English: p4-bin\bin.ntx64\en\dyn
   For Japanese: p4-bin\bin.ntx64\ja\dyn
   ```

## Install

To install the plugin in Autodesk Maya:

1. Copy the `.mll` file to the Maya Plug-ins directory.
2. Launch Maya and navigate to:
	```bash
 	Windows → Settings/Preferences → Plug-in Manager
 	```
3. In the Plug-in Manager dialog, search for P4GT-Maya-2026_x64.mll. Check Loaded to activate the plugin .Check Auto-Load to ensure it loads automatically on future launches

Once enabled, the P4 menu will appear in Maya’s main interface, allowing seamless Perforce integration.

## License

See `LICENSE.txt` for license information.
