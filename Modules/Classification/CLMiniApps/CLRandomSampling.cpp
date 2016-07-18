/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "mitkCommandLineParser.h"
#include "mitkIOUtil.h"
#include <mitkRandomImageSampler.h>

static vector<double> splitDouble(string str, char delimiter) {
  vector<double> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  double val;
  while (getline(ss, tok, delimiter)) {
    stringstream s2(tok);
    s2 >> val;
    internal.push_back(val);
  }

  return internal;
}

static vector<unsigned int> splitUInt(string str, char delimiter) {
  vector<unsigned int> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  unsigned int val;
  while (getline(ss, tok, delimiter)) {
    stringstream s2(tok);
    s2 >> val;
    internal.push_back(val);
  }

  return internal;
}

int main(int argc, char* argv[])
{
  mitkCommandLineParser parser;

  parser.setTitle("Random Sampling");
  parser.setCategory("Classification Command Tools");
  parser.setDescription("");
  parser.setContributor("MBI");

  parser.setArgumentPrefix("--", "-");
  // Add command line argument names
  parser.addArgument("help", "h", mitkCommandLineParser::Bool, "Help:", "Show this help text");
  parser.addArgument("input", "i", mitkCommandLineParser::InputDirectory, "Input file:", "Input file", us::Any(), false);
  parser.addArgument("output", "o", mitkCommandLineParser::OutputFile, "Output file:", "Output file", us::Any(), false);

  parser.addArgument("single-rate", "sr", mitkCommandLineParser::OutputFile, "Single Acceptance rate for all voxel", "Output file", us::Any(), true);
  parser.addArgument("class-rate", "cr", mitkCommandLineParser::OutputFile, "Class-dependend acceptance rate", "Output file", us::Any(), true);
  parser.addArgument("single-number", "sn", mitkCommandLineParser::OutputFile, "Single Number of Voxel for each class", "Output file", us::Any(), true);
  parser.addArgument("class-number", "cn", mitkCommandLineParser::OutputFile, "Class-dependedn number of voxels ", "Output file", us::Any(), true);

  map<string, us::Any> parsedArgs = parser.parseArguments(argc, argv);

  if (parsedArgs.size() == 0)
    return EXIT_FAILURE;

  // Show a help message
  if (parsedArgs.count("help") || parsedArgs.count("h"))
  {
    std::cout << parser.helpText();
    return EXIT_SUCCESS;
  }

  if (parsedArgs.count("single-rate") + parsedArgs.count("class-rate") + parsedArgs.count("single-number") + parsedArgs.count("class-number") < 1)
  {
    std::cout << "Please specify the sampling rate or number of voxels to be labeled" << std::endl << std::endl;
    std::cout << parser.helpText();
    return EXIT_SUCCESS;
  }

  if (parsedArgs.count("single-rate") + parsedArgs.count("class-rate") + parsedArgs.count("single-number") + parsedArgs.count("class-number") > 2)
  {
    std::cout << "Please specify only one way for the sampling rate or number of voxels to be labeled" << std::endl << std::endl;
    std::cout << parser.helpText();
    return EXIT_SUCCESS;
  }


  std::string inputName = us::any_cast<string>(parsedArgs["input"]);
  std::string outputName = us::any_cast<string>(parsedArgs["output"]);
  mitk::Image::Pointer image = mitk::IOUtil::LoadImage(inputName);

  mitk::RandomImageSampler::Pointer filter = mitk::RandomImageSampler::New();
  filter->SetInput(image);

  if (parsedArgs.count("single-rate"))
  {
    filter->SetSamplingMode(mitk::RandomImageSamplerMode::SINGLE_ACCEPTANCE_RATE);
    auto rate = splitDouble(parsedArgs["single-rate"].ToString(), ';');
    if (rate.size() != 1)
    {
      std::cout << "Please specify a single double value for single-rate, for example 0.3." << std::endl << std::endl;
      std::cout << parser.helpText();
      return EXIT_SUCCESS;
    }
    filter->SetAcceptanceRate(rate[0]);
  }

  else if (parsedArgs.count("class-rate"))
  {
    filter->SetSamplingMode(mitk::RandomImageSamplerMode::CLASS_DEPENDEND_ACCEPTANCE_RATE);
    auto rate = splitDouble(parsedArgs["class-rate"].ToString(), ';');
    if (rate.size() < 2)
    {
      std::cout << "Please specify at least two, semicolon separated values for class-rate, for example '0.3;0.2' ." << std::endl << std::endl;
      std::cout << parser.helpText();
      return EXIT_SUCCESS;
    }
    filter->SetAcceptanceRateVector(rate);
  }

  else if (parsedArgs.count("single-number"))
  {
    filter->SetSamplingMode(mitk::RandomImageSamplerMode::SINGLE_NUMBER_OF_ACCEPTANCE);
    auto rate = splitUInt(parsedArgs["single-number"].ToString(), ';');
    if (rate.size() != 1)
    {
      std::cout << "Please specify a single double value for single-number, for example 100." << std::endl << std::endl;
      std::cout << parser.helpText();
      return EXIT_SUCCESS;
    }
    filter->SetNumberOfSamples(rate[0]);
  }

  else if (parsedArgs.count("class-number"))
  {
    filter->SetSamplingMode(mitk::RandomImageSamplerMode::CLASS_DEPENDEND_NUMBER_OF_ACCEPTANCE);
    auto rate = splitUInt(parsedArgs["class-number"].ToString(), ';');
    if (rate.size() < 2)
    {
      std::cout << "Please specify at least two, semicolon separated values for class-number, for example '100;200' ." << std::endl << std::endl;
      std::cout << parser.helpText();
      return EXIT_SUCCESS;
    }
    filter->SetNumberOfSamplesVector(rate);
  }
  filter->Update();
  mitk::IOUtil::SaveImage(filter->GetOutput(), outputName);

  return EXIT_SUCCESS;
}