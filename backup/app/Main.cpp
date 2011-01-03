///
/// @file Main.cpp
/// @author Mancobian Poemandres
/// @license BSD License
///
/// Copyright (c) MMX by The Secret Design Collective
/// All rights reserved
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
///    * Redistributions of source code must retain the above copyright notice,
/// 		this list of conditions and the following disclaimer.
///    * Redistributions in binary form must reproduce the above copyright notice,
/// 		this list of conditions and the following disclaimer in the documentation
/// 		and/or other materials provided with the distribution.
///    * Neither the name of The Secret Design Collective nor the names of its
/// 		contributors may be used to endorse or promote products derived from
/// 		this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
/// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///

#include <iostream>
#include <app/ogre/OgreApplication.h>

using namespace Nous;
using namespace Nous::App;

// TODO: Integrate this as a virtual function in IApp
void PrintUsage()
{
	using namespace std;
	cout << endl;
	cout << "Demo_DeferredLighting v1.0" << endl;
	cout << "==========================" << endl;
	cout << left << endl;
	cout << setw(25) << "Usage:" << "Demo_DeferredLighting [options]" << endl;
	cout << setw(25) << "Options:" << endl;
	cout << setw(25) << "  --scene [filename]";
	cout << setw(55) << "The absolute path of a scene file to load." << endl;
	cout << endl;
}

int main(int argc, char **argv)
{
	try
	{
		// Local vars
		std::string scene_file, dicom_file;

		// Parse command line args
		for (int i = 0; i < argc; ++i)
		{
			if (strcmp(argv[i], "--scene") == 0)
			{
				scene_file = argv[++i];
			}
			else if (strcmp(argv[i], "--dicom") == 0)
			{
				dicom_file = argv[++i];
			}
		}

		// Create the main app
		OgreApplication app;

		// Specify scene file for loading
		if (!scene_file.empty())
			app.setSceneName(scene_file);

		// Run the application
		app.start();
	}
	catch (const std::string &e)
	{
		// Display exception message
		fprintf(stderr, "An exception has occurred: %s\n", e.c_str());

		// Print usage and exit!
		PrintUsage();
	}
	return 0;
}
