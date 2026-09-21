//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2026 Ryo Suzuki
//	Copyright (c) 2016-2026 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <unistd.h>
# include <filesystem>
# include <Siv3D/FileSystem.hpp>
# include <Siv3D/EnvironmentVariable.hpp>
# include <Siv3D/SpecialFolder.hpp>
# include <Siv3D/Unicode.hpp>

namespace s3d
{
	namespace detail
	{
		[[nodiscard]]
		static std::filesystem::path ToPath(const FilePathView path)
		{
			return std::filesystem::path{ Unicode::ToUTF8(path) };
		}

		[[nodiscard]]
		static FilePath FromPath(const std::filesystem::path& path)
		{
			return Unicode::FromUTF8(path.string());
		}

		namespace init
		{
			const static FilePath g_launchDirectory = FileSystem::CurrentDirectory();

			const static FilePath g_executablePath = U"/a.out";
			const static FilePath g_executableDirectory = U"/";

			const static std::array<FilePath, 12> g_specialFolderPaths = []()
			{
				FilePath homeDirectory = EnvironmentVariable::Get(U"HOME");
				if (not homeDirectory.ends_with(U'/'))
				{
					homeDirectory.push_back(U'/');
				}

				std::array<FilePath, 12> specialFolderPaths;

				// specialFolderPaths[FromEnum(SpecialFolder::Desktop)]      = U"";
				// specialFolderPaths[FromEnum(SpecialFolder::Documents)]    = U"";
				specialFolderPaths[FromEnum(SpecialFolder::LocalAppData)] = U"/var/cache/";
				// specialFolderPaths[FromEnum(SpecialFolder::Pictures)]     = U"";
				// specialFolderPaths[FromEnum(SpecialFolder::Music)]        = U"";
				// specialFolderPaths[FromEnum(SpecialFolder::Videos)]       = U"";
				// specialFolderPaths[FromEnum(SpecialFolder::SystemFonts)]  = U"";
				// specialFolderPaths[FromEnum(SpecialFolder::LocalFonts)]   = U"";
				// specialFolderPaths[FromEnum(SpecialFolder::UserFonts)]    = U"";
				specialFolderPaths[FromEnum(SpecialFolder::UserProfile)]  = homeDirectory;
				// specialFolderPaths[FromEnum(SpecialFolder::ProgramFiles)] = U"";
				// specialFolderPaths[FromEnum(SpecialFolder::Downloads)]    = U"";

				return specialFolderPaths;
			}();

			const static Array<FilePath> g_resourceFilePaths = []()
			{
				Array<FilePath> paths = FileSystem::DirectoryContents(U"/resources/", Recursive::Yes);

				paths.erase_all_if(FileSystem::IsDirectory);

				paths.sort();

				return paths;
			}();

			const Array<FilePath>& GetResourceFiles() noexcept
			{
				return g_resourceFilePaths;
			}
		}
	}

	namespace FileSystem
	{
		////////////////////////////////////////////////////////////////
		//
		//	IsResourcePath
		//
		////////////////////////////////////////////////////////////////

		bool IsResourcePath(const FilePathView path) noexcept
		{
			return FullPath(path).starts_with(U"/resources/");
		}

		////////////////////////////////////////////////////////////////
		//
		//	Exists
		//
		////////////////////////////////////////////////////////////////

		bool Exists(const FilePathView path)
		{
			if (path.isEmpty())
			{
				return false;
			}

			return exists(detail::ToPath(path));
		}

		////////////////////////////////////////////////////////////////
		//
		//	IsDirectory
		//
		////////////////////////////////////////////////////////////////

		bool IsDirectory(const FilePathView path)
		{
			if (path.isEmpty())
			{
				return false;
			}

			return is_directory(detail::ToPath(path));
		}

		////////////////////////////////////////////////////////////////
		//
		//	IsFile
		//
		////////////////////////////////////////////////////////////////

		bool IsFile(const FilePathView path)
		{
			if (path.isEmpty())
			{
				return false;
			}

			return is_regular_file(detail::ToPath(path));
		}

		////////////////////////////////////////////////////////////////
		//
		//	IsResource
		//
		////////////////////////////////////////////////////////////////

		bool IsResource(const FilePathView path)
		{
			return (IsResourcePath(path) && Exists(path));
		}

		////////////////////////////////////////////////////////////////
		//
		//	NativePath
		//
		////////////////////////////////////////////////////////////////

		NativeFilePath NativePath(const FilePathView path)
		{
			if (path.isEmpty())
			{
				return{};
			}

			return weakly_canonical(detail::ToPath(path)).native();
		}

		////////////////////////////////////////////////////////////////
		//
		//	VolumePath
		//
		////////////////////////////////////////////////////////////////

		FilePath VolumePath(const FilePathView)
		{
			return U"/";
		}

		////////////////////////////////////////////////////////////////
		//
		//	DirectoryContents
		//
		////////////////////////////////////////////////////////////////

		Array<FilePath> DirectoryContents(const FilePathView path, const Recursive recursive)
		{
			if (path.isEmpty() || (not IsDirectory(path)))
			{
				return{};
			}

			Array<FilePath> paths;

			if (recursive)
			{
				for (const auto& v : std::filesystem::recursive_directory_iterator{ detail::ToPath(path) })
				{
					paths.push_back(FullPath(detail::FromPath(v.path())));
				}
			}
			else
			{
				for (const auto& v : std::filesystem::directory_iterator{ detail::ToPath(path) })
				{
					paths.push_back(FullPath(detail::FromPath(v.path())));
				}
			}

			return paths;
		}

		////////////////////////////////////////////////////////////////
		//
		//	GetLaunchDirectory
		//
		////////////////////////////////////////////////////////////////

		const FilePath& GetLaunchDirectory() noexcept
		{
			return detail::init::g_launchDirectory;
		}

		////////////////////////////////////////////////////////////////
		//
		//	GetExecutablePath
		//
		////////////////////////////////////////////////////////////////

		const FilePath& GetExecutablePath() noexcept
		{
			return detail::init::g_executablePath;
		}

		////////////////////////////////////////////////////////////////
		//
		//	GetExecutableDirectory
		//
		////////////////////////////////////////////////////////////////

		const FilePath& GetExecutableDirectory() noexcept
		{
			return detail::init::g_executableDirectory;
		}

		////////////////////////////////////////////////////////////////
		//
		//	ChangeCurrentDirectory
		//
		////////////////////////////////////////////////////////////////

		bool ChangeCurrentDirectory(const FilePathView path)
		{
			if (not IsDirectory(path))
			{
				return false;
			}

			return (::chdir(Unicode::ToUTF8(path).c_str()) == 0);
		}

		////////////////////////////////////////////////////////////////
		//
		//	GetFolderPath
		//
		////////////////////////////////////////////////////////////////

		const FilePath& GetFolderPath(const SpecialFolder folder)
		{
			assert(FromEnum(folder) < static_cast<int32>(std::size(detail::init::g_specialFolderPaths)));

			return detail::init::g_specialFolderPaths[FromEnum(folder)];
		}

		////////////////////////////////////////////////////////////////
		//
		//	Remove
		//
		////////////////////////////////////////////////////////////////

		bool Remove(const FilePathView path, const MoveToTrash moveToTrash)
		{
			if (path.isEmpty())
			{
				return false;
			}

			if (moveToTrash)
			{
				// Not supported
				return false;
			}

			try
			{
				remove_all(detail::ToPath(path));
				return true;
			}
			catch (const std::filesystem::filesystem_error&)
			{
				return false;
			}
		}
	}
}
