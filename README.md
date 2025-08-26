## Detector 🛡️
### A simple, real-time malware detection engine written in C++. This project actively monitors a specified directory (e.g., the Downloads folder) and scans new files against a database of known malware signatures.

## 📜 Description
### This project is an educational tool designed to demonstrate the core principles of signature-based malware detection. It runs as a background console application that watches for newly created files. When a new file appears, the application immediately calculates its SHA256 hash and compares it against a list of known malicious hashes. If a match is found, it flags the file as malware.

## ✨ Features
###  Real-Time Monitoring: Uses the native Windows API to efficiently monitor a directory for new files without constant polling.

###  Signature-Based Scanning: Employs SHA256 hashing to quickly and accurately identify files based on their unique digital fingerprints.

###  Lightweight & Performant: Written in C++ for minimal resource consumption.

###  Simple Signature Database: Uses a basic <mark>.db</mark> text file, making it easy to add new malware signatures.

###  Self-Contained: Includes a public-domain SHA256 implementation, requiring no external cryptography libraries.

## ⚙️ How It Works
### The detection process follows three main steps:

###  Monitor: The <mark>Monitor</mark> class uses the Windows <mark>ReadDirectoryChangesW</mark> function to receive notifications from the operating system whenever a file is added to the target directory.

### Hash: When a new file is detected, the <amrk>Scanner</mark> class reads the file's binary content and computes its SHA256 hash.

### Compare: The calculated hash is then compared against the list of signatures loaded from the <mark>signatures.db</mark> file. If the hash exists in the signature set, the file is flagged as malicious.

## 🛠️ Tech Stack
### Language: C++ (C++17)

### Build System: CMake

### Core Functionality: Standard Template Library (STL)

### File Monitoring: Native Windows API (<windows.h>)

## 📋 Prerequisites
###  Before you begin, ensure you have the following installed on your Windows machine:

###  A C++ Compiler: The MSVC compiler included with the "Desktop development with C++" workload in Visual Studio is recommended.

###  CMake: Download from the official CMake website. Important: During installation, make sure to select the option to "Add CMake to the system PATH".


## 🚀 Getting Started
### Follow these instructions to compile and run the project on your local machine.

### 1. Clone the Repository
     git clone https://github.com/krishna-Agrawal23/Detector.git
     cd Detector

### 2. Configure the Monitored Directory
#### Open the <mark>main.cpp</mark> file and change the <mark>pathToWatch</mark> variable to the absolute path of the directory you want to monitor (e.g., your Downloads folder).

    // In main.cpp
    std::string pathToWatch = "A:\\Downloads\\Chrome";          // Change this path!

### 3. Build the Project

### Use CMake to generate the build files and compile the source code.

      Create a build directory
      mkdir build
      cd build

 ### Generate the build files
      cmake ..

### Compile the project
      cmake --build .

### This will create <mark>Detector.exe</mark> inside the <mark>build/Debug</mark> directory.

### 4. Run the Application
#### 1. Navigate to the <mark>Debug</mark> directory:
    cd Debug

#### 2. Copy the signature database into this directory so the executable can find it:
      copy ..\..\signatures.db .

#### 3 Run the executable:
      Detector.exe

The application is now running and monitoring your specified folder.

## 🧪 How to Test
### You can test the detector by creating a sample <mark>"malicious"</mark> file.

### 1 Create a new text file named <mark>virus.txt</mark>.

### 2. Use an online tool or a local utility to find the SHA256 hash of this empty file. The hash for a completely empty file is:

     e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855

### 3. Ensure this hash is present in your signatures.db file.

### 4. Copy <mark>virus.txt</mark> into the directory you are monitoring.

### 5. The console window running the detector should immediately print a <mark>!!! MALWARE DETECTED !!!</mark> message.

## ⚠️ Disclaimer
### This is a proof-of-concept project and is not intended for real-world security protection. It lacks many features of a commercial antivirus, such as heuristic analysis, sandboxing, and a comprehensive, auto-updating signature database.

