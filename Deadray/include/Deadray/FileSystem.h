#ifndef   DEADRAY_FILE_SYSTEM_H
#define   DEADRAY_FILE_SYSTEM_H

namespace Deadray {
	class FileSystem {
	public:
	  AppFileSystem(const char* packFile);
	  ~AppFileSystem();
	 
	  ///
	  /// Check if file path exists, return true if exists.
	  ///
	  virtual bool FileExists(const String16& path) override;

	  ///
	  /// Get file size of previously opened file, store result in 'result'. Return true on success.
	  ///
	  virtual bool GetFileSize(FileHandle handle, int64_t& result) override;

	  ///
	  /// Get file mime type (eg "text/html"), store result in 'result'. Return true on success.
	  ///
	  virtual bool GetFileMimeType(const String16& path, String16& result) override;

	  ///
	  /// Open file path for reading or writing. Return file handle on success, or invalidFileHandle on failure.
	  ///
	  /// @NOTE:  As of this writing (v1.2), this function is only used for reading.
	  ///
	  virtual FileHandle OpenFile(const String16& path, bool open_for_writing) override;

	  ///
	  /// Close previously-opened file.
	  ///
	  virtual void CloseFile(FileHandle& handle) override;

	  ///
	  /// Read from currently-opened file, return number of bytes read or -1 on failure.
	  ///
	  virtual int64_t ReadFromFile(FileHandle handle, char* data, int64_t length) override;

	protected:
		HANDLE h_pakFile;
		std::vector<uint32_t> chunkHashes;
		std::vector<uint32_t> chunkOffsets;
		std::vector<uint32_t> chunkSizes;
		std::vector<uint32_t> chunkSizesDeflated;

		ThumbnailCacheFile* thumbCache = nullptr;
	};
}

#endif