// implement this class so that you can partake in ESortedFileList

#ifndef FileSpecProviderINC
#define FileSpecProviderINC

class FileSpecProvider {
	public:
		virtual HORef<MFileSpec>	GetFileSpec(void) const = 0;
};//end class FileSpecProvider

#endif