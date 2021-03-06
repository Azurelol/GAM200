/*!****************************************************************************\
@file   FileSystem.cpp
@author Christian Sagel
@author Johannes Peter
@brief  A BOOST-powered interface static class that provides common and useful
filesystem operations (such as scanning folders, etc)
@note   The Boost Filesystem library:
http://www.boost.org/doc/libs/1_39_0/libs/filesystem/doc/index.htm
Great reference for using the library:
http://www.technical-recipes.com/2014/using-boostfilesystem/
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

\******************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <BOOST/filesystem.hpp>


namespace DCEngine
{
  using filepath = boost::filesystem::path;

  class FileSystem
  {
    public:
      /* Shell */
      static bool Execute(std::string command);
      static bool Execute(const std::string& command, const std::vector<std::string>& arguments);

      /* File Dialog */
      static std::string  FileOpenDialog(std::string& defaultPath, std::string& filters);
      static std::string  FileSaveDialog(std::string& defaultPath, std::string& filters);

      /* Path Operations*/
      static bool         Exists(filepath path);
      static std::time_t  LastModified(filepath path);
      static bool         Delete(filepath path);
      static std::string  CorrectPath(std::string&);
      static std::string  RelativePath(const std::string& path, const std::string& pathRelativeTo);

      /* File Operations */
      static bool         FileCreate(filepath filePath);
      static int          FileSize(filepath filePath);
      static bool         FileRename(filepath filePath, std::string newFileName);
      static bool         FileReadToString(filepath filePath, std::string& output);
      static bool         FileWriteString(filepath filePath, std::string& input, bool append = false);
      static std::string  FileNoExtension(filepath filePath);
      static std::string  FileExtension(filepath filePath);
      static bool         FileHasExtension(filepath filePath, std::string extension);
      static std::string  FileNoPath(filepath filePath);   

      /* Folder Operations */
      static bool         DirectoryCreate(filepath dirPath);
      static bool         DirectoryListPaths(filepath dirPath, std::vector<filepath>& paths);
      static bool         DirectoryListFiles(filepath dirPath, std::vector<filepath>& files);
      static bool         DirectoryListFileNames(std::string dirPath, std::vector<std::string>& fileNames);
      static bool         DirectoryListFilePaths(std::string dirPath, std::vector<std::string>& filePaths);
      static bool         DirectoryListFilePaths(std::string dirPath, std::vector<std::string>& filePaths, std::string extension);
      static bool         DirectoryListDirs(filepath dirPath, std::vector<filepath>& dirs);
      static bool         DirectoryContainsFile(filepath dirPath, std::string fileName, std::string& foundPath);
      static unsigned int DirectoryCountFiles(filepath dirPath);
      static unsigned int DirectoryCountDirs(filepath dirPath);
  };

  struct FileInfo {
    filepath FilePathAbs; // The absolute path of the file
    std::string FileName;      //                        Example: foo.bar
    std::string FileStem;      // Basename of the file.  Example: foo
    std::string Extension;     // Extension of the file. Example: bar
    std::time_t LastModified;  
    FileInfo() : LastModified(0) {} 
    FileInfo(filepath path) :
      
      // Old
      //FilePathAbs(Utils::String::ToLower(path.string())),
      //FileName(Utils::String::ToLower(path.filename().string())),
      //FileStem(Utils::String::ToLower(path.stem().string())),
      //Extension(Utils::String::ToLower(path.extension().string())),

      FilePathAbs(path.string()),
      FileName(path.filename().string()),
      FileStem(path.stem().string()),
      Extension(path.extension().string()),
      LastModified(FileSystem::LastModified(path)) {}

  };

}
