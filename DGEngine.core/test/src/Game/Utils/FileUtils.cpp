#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"

#include "Game/Utils/FileUtils.h"
#include <physfs.h>

TEST_CASE("FileUtils::exists", "GameUtils")
{
	CHECK(FileUtils::exists("text.txt") == true);
	CHECK(FileUtils::exists("unknown.file") == false);
}

TEST_CASE("FileUtils::makePhysfsPath", "GameUtils")
{
	CHECK(FileUtils::makePhysfsPath("", "file.txt") == "file.txt");
	CHECK(FileUtils::makePhysfsPath(".", "file.txt") == "./file.txt");
	CHECK(FileUtils::makePhysfsPath("path", "file.txt") == "path/file.txt");
	CHECK(FileUtils::makePhysfsPath("path/", "file.txt") == "path/file.txt");
	CHECK(FileUtils::makePhysfsPath("path\\", "file.txt") == "path\\/file.txt");
}

TEST_CASE("FileUtils::makeSystemPath", "GameUtils")
{
	CHECK(FileUtils::makeSystemPath("", "file.txt") == "file.txt");
	CHECK(FileUtils::makeSystemPath(".", "file.txt") == std::string(".") + PHYSFS_getDirSeparator() + "file.txt");
	CHECK(FileUtils::makeSystemPath("path", "file.txt") == std::string("path") + PHYSFS_getDirSeparator() + "file.txt");
	CHECK(FileUtils::makeSystemPath("path/", "file.txt") == "path/file.txt");
	CHECK(FileUtils::makeSystemPath("path\\", "file.txt") == "path\\file.txt");
}

TEST_CASE("FileUtils::getDirList", "GameUtils")
{
	CHECK(::Catch::Detail::stringify(FileUtils::geDirList("", false)) == R"({ "dir1", "dir2", "json", "res" })");
	CHECK(::Catch::Detail::stringify(FileUtils::geDirList("dir1", false)) == R"({ "subdir1", "subdir2" })");
	CHECK(::Catch::Detail::stringify(FileUtils::geDirList("unknownDir", false)) == "{  }");
}

TEST_CASE("FileUtils::getFileList", "GameUtils")
{
	CHECK(::Catch::Detail::stringify(FileUtils::getFileList("", "", false)) == R"({ "text.txt" })");
	CHECK(::Catch::Detail::stringify(FileUtils::getFileList("dir1", "", false)) == R"({ "file1.txt", "file2.json" })");
	CHECK(::Catch::Detail::stringify(FileUtils::getFileList("dir1", "txt", false)) == R"({ "file1.txt" })");
	CHECK(::Catch::Detail::stringify(FileUtils::getFileList("dir1", ".txt", false)) == R"({ "file1.txt" })");
	CHECK(::Catch::Detail::stringify(FileUtils::getFileList("dir1", "file1.txt", false)) == R"({ "file1.txt" })");
	CHECK(::Catch::Detail::stringify(FileUtils::getFileList("dir1", "TXT", false)) == "{  }");
	CHECK(::Catch::Detail::stringify(FileUtils::getFileList("dir1", "json", false)) == R"({ "file2.json" })");
	CHECK(::Catch::Detail::stringify(FileUtils::getFileList("dir1", "json", true)) == R"({ "dir1/file2.json" })");
}

TEST_CASE("FileUtils::getFileName", "GameUtils")
{
	CHECK(FileUtils::getFileName("file1") == "file1");
	CHECK(FileUtils::getFileName("file1.txt") == "file1.txt");
	CHECK(FileUtils::getFileName("/mnt/file1.txt") == "file1.txt");
	CHECK(FileUtils::getFileName("../file1.txt") == "file1.txt");
	CHECK(FileUtils::getFileName("../.txt") == ".txt");
}

TEST_CASE("FileUtils::getFileNameWithoutExt", "GameUtils")
{
	CHECK(FileUtils::getFileNameWithoutExt("file1") == "file1");
	CHECK(FileUtils::getFileNameWithoutExt("file1.txt") == "file1");
	CHECK(FileUtils::getFileNameWithoutExt("/mnt/file1.txt") == "file1");
	CHECK(FileUtils::getFileNameWithoutExt("../file1.txt") == "file1");
	CHECK(FileUtils::getFileNameWithoutExt("../.txt") == ".txt");
}

TEST_CASE("FileUtils::getFileExtension", "GameUtils")
{
	CHECK(FileUtils::getFileExtension("file1") == "");
	CHECK(FileUtils::getFileExtension("file1 ") == "");
	CHECK(FileUtils::getFileExtension("file1.") == ".");
	CHECK(FileUtils::getFileExtension("file1. ") == ". ");
	CHECK(FileUtils::getFileExtension("file1.txt") == ".txt");
	CHECK(FileUtils::getFileExtension("/mnt/file1.txt") == ".txt");
	CHECK(FileUtils::getFileExtension("../file1.txt") == ".txt");
}

TEST_CASE("FileUtils::getFilePath", "GameUtils")
{
	CHECK(FileUtils::getFilePath("file1.txt") == "");
	CHECK(FileUtils::getFilePath("/mnt/file1.txt") == "/mnt");
	CHECK(FileUtils::getFilePath("../file1.txt") == "..");
	CHECK(FileUtils::getFilePath("dir1/../dir1/file1.txt") == "dir1/../dir1");
}

TEST_CASE("FileUtils::readBytes", "GameUtils")
{
	SECTION("filename")
	{
		auto bytes = FileUtils::readBytes("text.txt");
		CHECK(::Catch::Detail::stringify(bytes) == "{ 116, 104, 105, 115, 32, 105, 115, 32, 97, 32, 116, 101, 120, 116, 46, 10 }");

		bytes = FileUtils::readBytes("unknown.file");
		CHECK(::Catch::Detail::stringify(bytes) == "{  }");
	}
	SECTION("filename, maxNumBytes")
	{
		auto bytes = FileUtils::readBytes("text.txt", 0);
		CHECK(::Catch::Detail::stringify(bytes) == "{  }");

		bytes = FileUtils::readBytes("text.txt", 2);
		CHECK(::Catch::Detail::stringify(bytes) == "{ 116, 104 }");

		bytes = FileUtils::readBytes("text.txt", 50);
		CHECK(::Catch::Detail::stringify(bytes) == "{ 116, 104, 105, 115, 32, 105, 115, 32, 97, 32, 116, 101, 120, 116, 46, 10 }");

		bytes = FileUtils::readBytes("unknown.file", 0);
		CHECK(::Catch::Detail::stringify(bytes) == "{  }");

		bytes = FileUtils::readBytes("unknown.file", 2);
		CHECK(::Catch::Detail::stringify(bytes) == "{  }");
	}
	SECTION("filename, startPosition, maxNumBytes")
	{
		auto bytes = FileUtils::readBytes("text.txt", 2, 0);
		CHECK(::Catch::Detail::stringify(bytes) == "{  }");

		bytes = FileUtils::readBytes("text.txt", 2, 2);
		CHECK(::Catch::Detail::stringify(bytes) == "{ 105, 115 }");

		bytes = FileUtils::readBytes("text.txt", 15, 2);
		CHECK(::Catch::Detail::stringify(bytes) == "{ 10 }");

		bytes = FileUtils::readBytes("text.txt", 2, 50);
		CHECK(::Catch::Detail::stringify(bytes) == "{ 105, 115, 32, 105, 115, 32, 97, 32, 116, 101, 120, 116, 46, 10 }");

		bytes = FileUtils::readBytes("text.txt", 50, 0);
		CHECK(::Catch::Detail::stringify(bytes) == "{  }");

		bytes = FileUtils::readBytes("text.txt", 50, 2);
		CHECK(::Catch::Detail::stringify(bytes) == "{  }");

		bytes = FileUtils::readBytes("unknown.file", 0, 2);
		CHECK(::Catch::Detail::stringify(bytes) == "{  }");

		bytes = FileUtils::readBytes("unknown.file", 2, 2);
		CHECK(::Catch::Detail::stringify(bytes) == "{  }");
	}
}

TEST_CASE("FileUtils::readText", "GameUtils")
{
	std::string str = "this is a text.\n";

	CHECK(FileUtils::readText("text.txt") == str);
	CHECK(FileUtils::readText("unknown.file") == "");
}

TEST_CASE("FileUtils::getSaveDir", "GameUtils")
{
	SECTION("uninitialized")
	{
		CHECK(PHYSFS_setWriteDir(nullptr) != 0);
		CHECK(FileUtils::getSaveDir() == "");
	}
	SECTION("initialized")
	{
		CHECK(FileUtils::setSaveDir("invalidDir") == true);
		CHECK_THAT(FileUtils::getSaveDir(), Catch::Matchers::EndsWith(std::string("invalidDir") + PHYSFS_getDirSeparator()));
		CHECK(FileUtils::setSaveDir("dir1") == true);
		CHECK_THAT(FileUtils::getSaveDir(), Catch::Matchers::EndsWith(std::string("dir1") + PHYSFS_getDirSeparator()));
		CHECK(FileUtils::setSaveDir(nullptr) == false);
		CHECK(FileUtils::setSaveDir("") == false);
		CHECK_THAT(FileUtils::getSaveDir(), Catch::Matchers::EndsWith(std::string("dir1") + PHYSFS_getDirSeparator()));
	}
}

TEST_CASE("FileUtils::CRUD", "GameUtils")
{
	SECTION("initialize")
	{
		CHECK(FileUtils::setSaveDir("dir1") == true);
	}
	SECTION("create dir")
	{
		CHECK(FileUtils::createDir("newDir") == true);
		CHECK(FileUtils::createDir("newDir/subDir") == true);
		CHECK(FileUtils::exists("newDir") == false);
		CHECK(FileUtils::exists("newDir/subDir") == false);
	}
	SECTION("mount save dir")
	{
		CHECK(FileUtils::mount(FileUtils::getSaveDir(), "save", false) == true);
		CHECK(FileUtils::exists("save/newDir") == true);
		CHECK(FileUtils::exists("save/newDir/subDir") == true);
	}
	SECTION("create files")
	{
		FileBytes bytes = { (std::byte)1, (std::byte)2, (std::byte)3 };

		CHECK(FileUtils::saveBytes("newFile.bin", bytes) == true);
		CHECK(FileUtils::exists("save/newFile.bin") == true);

		CHECK(FileUtils::saveText("newFile.txt", "saving text...") == true);
		CHECK(FileUtils::exists("save/newFile.txt") == true);
	}
	SECTION("read files")
	{
		auto bytes = FileUtils::readBytes("save/newFile.bin");
		CHECK(::Catch::Detail::stringify(bytes) == "{ 1, 2, 3 }");

		CHECK(FileUtils::readText("save/newFile.txt") == "saving text...");
	}
	SECTION("read save dir")
	{
		CHECK(::Catch::Detail::stringify(FileUtils::geDirList("", false)) == R"({ "dir1", "dir2", "json", "res", "save" })");
		CHECK(::Catch::Detail::stringify(FileUtils::geDirList("", true)) == R"({ "save" })");
		CHECK(::Catch::Detail::stringify(FileUtils::geDirList("save", false)) == R"({ "newDir" })");
		CHECK(::Catch::Detail::stringify(FileUtils::geDirList("save", true)) == R"({ "newDir" })");
	}
	SECTION("overwrite files")
	{
		FileBytes bytes = { (std::byte)4, (std::byte)5, (std::byte)6 };

		CHECK(FileUtils::saveBytes("newFile.bin", bytes) == true);
		CHECK(FileUtils::exists("save/newFile.bin") == true);

		CHECK(FileUtils::saveText("newFile.txt", "updating text...") == true);
		CHECK(FileUtils::exists("save/newFile.txt") == true);
	}
	SECTION("read files")
	{
		auto bytes = FileUtils::readBytes("save/newFile.bin");
		CHECK(::Catch::Detail::stringify(bytes) == "{ 4, 5, 6 }");

		CHECK(FileUtils::readText("save/newFile.txt") == "updating text...");
	}
	SECTION("delete files")
	{
		CHECK(FileUtils::deleteFile("save/invalidFile.txt") == false);
		CHECK(FileUtils::deleteFile("save/newFile.bin") == false);

		// save dir should only be mounted in "", so unmount "save" mountPoint and mount it in ""
		CHECK(FileUtils::unmount(FileUtils::getSaveDir()) == true);
		CHECK(FileUtils::mount(FileUtils::getSaveDir(), "", false) == true);

		CHECK(::Catch::Detail::stringify(FileUtils::geDirList("", false)) == R"({ "dir1", "dir2", "json", "newDir", "res" })");
		CHECK(::Catch::Detail::stringify(FileUtils::geDirList("", true)) == R"({ "newDir" })");
		CHECK(::Catch::Detail::stringify(FileUtils::geDirList("newDir", false)) == R"({ "subDir" })");
		CHECK(::Catch::Detail::stringify(FileUtils::geDirList("newDir", true)) == R"({ "subDir" })");

		CHECK(FileUtils::exists("newDir") == true);
		CHECK(FileUtils::exists("newFile.bin") == true);

		CHECK(FileUtils::deleteFile("newFile.bin") == true);

		CHECK(FileUtils::exists("newFile.bin") == false);
		CHECK(FileUtils::exists("newFile.txt") == true);

		CHECK(FileUtils::deleteAll("", false) == true);

		CHECK(FileUtils::exists("newDir") == false);
		CHECK(FileUtils::exists("newFile.txt") == false);
		CHECK(FileUtils::exists("text.txt") == true);
	}
	SECTION("unmount save dir")
	{
		CHECK(FileUtils::unmount(FileUtils::getSaveDir()) == true);
		CHECK(FileUtils::exists("text.txt") == true);
	}
}
