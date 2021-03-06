//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.apache.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------

//!
//! @author David Lee
//! @date   2017 Aug 3rd
//!
//! Unit Test for NGSS Config
//!
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/filesystem/path.h>

#ifdef DISABLE_BIOGEARS_FileUtils_TEST
#define TEST_FIXTURE_NAME DISABLED_FileUtilsFixture
#else
#define TEST_FIXTURE_NAME FileUtilsFixture
#endif

// The fixture for testing class Foo.
class TEST_FIXTURE_NAME : public ::testing::Test {
protected:
  // You can do set-up work for each test here.
  TEST_FIXTURE_NAME() = default;

  // You can do clean-up work that doesn't throw exceptions here.
  virtual ~TEST_FIXTURE_NAME() = default;

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  // Code here will be called immediately after the constructor (right
  // before each test).
  virtual void SetUp();

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown();
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{
   biogears::SetCurrentWorkingDirectory("");
}

TEST_F(TEST_FIXTURE_NAME, FileUtils_CWD_Change)
{
  using namespace biogears;
  EXPECT_EQ("", biogears::GetCurrentWorkingDirectory());
#ifdef _WIN32
  biogears::SetCurrentWorkingDirectory("C:/");
  EXPECT_EQ("C:/", biogears::GetCurrentWorkingDirectory());
#else
  SetCurrentWorkingDirectory("/opt/biogears");
  EXPECT_EQ("/opt/biogears", biogears::GetCurrentWorkingDirectory());
#endif
}

TEST_F(TEST_FIXTURE_NAME, FileUtils_ResolvePath)
{
  using namespace biogears;
  using namespace biogears::filesystem;
  biogears::SetCurrentWorkingDirectory("");
  EXPECT_EQ(path::getcwd() / "test_file.txt", ResolvePath("test_file.txt"));
  EXPECT_EQ(path::getcwd() / "test_file.txt", ResolvePath("./test_file.txt"));

#ifdef _WIN32
  biogears::SetCurrentWorkingDirectory("C:/");
  EXPECT_EQ(path("C:/test_file.txt").make_normal(), ResolvePath("test_file.txt"));
  EXPECT_EQ(path("C:/test_file.txt").make_normal(), ResolvePath("./test_file.txt"));
  EXPECT_EQ(path("C:/biogears/test_file.txt").make_normal(), ResolvePath("C:/biogears/test_file.txt"));
#else
  SetCurrentWorkingDirectory("/opt/biogears");
  EXPECT_EQ(path("/opt/biogears/test_file.txt").make_normal(), ResolvePath("test_file.txt"));
  EXPECT_EQ(path("/opt/biogears/test_file.txt").make_normal(), ResolvePath("./test_file.txt"));
  EXPECT_EQ(path("/opt/biogears/biogears/test_file.txt").make_normal(), ResolvePath("/opt/biogears/biogears/test_file.txt"));
#endif

  auto cwd = path::getcwd().parent_path();
  biogears::SetCurrentWorkingDirectory("..");
  EXPECT_EQ((cwd/"test_file.txt").make_normal(), ResolvePath("test_file.txt"));
  EXPECT_EQ((cwd/"test_file.txt").make_normal(), ResolvePath("./test_file.txt"));
#ifdef _WIN32
  EXPECT_EQ(path("C:/biogears/test_file.txt").make_normal(), ResolvePath("C:\\biogears\\test_file.txt"));
#else
  EXPECT_EQ(path("/opt/biogears/test_file.txt").make_normal(), ResolvePath("/opt/biogears/test_file.txt"));
#endif
}
