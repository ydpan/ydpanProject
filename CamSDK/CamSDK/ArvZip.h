#ifndef _LP_ZIP_H_
#define _LP_ZIP_H_

#include "BaseDataDefine.h"
#include <vector>
struct ZipFile {
	char *name;
	size_t compressed_size;
	size_t uncompressed_size;
	ptrdiff_t offset;
};


class ArvZip
{
public:
	ArvZip(const void *buffer, size_t size);
	~ArvZip();

	const char *arv_zip_file_get_name(ZipFile *zip_file);
	size_t arv_zip_file_get_uncompressed_size(ZipFile *zip_file);
	void arv_zip_build_file_list();
	ZipFile *arv_zip_find_file(const char *name);
	ptrdiff_t arv_zip_get_file_data(ZipFile *zip_file);
	void *arv_zip_get_file(const char *name, size_t *size);
	const std::vector<ZipFile*> arv_zip_get_file_list();

	static inline lp_uint32 arv_guint32_from_unaligned_le_ptr(const char *ptr, lp_int32 offset)
	{
		lp_int32 val;
		if (ptr == NULL)
			return 0;

		ptr += offset;

		*((char*)(&val)) = *((char*)ptr);
		*(((char*)(&val)) + 1) = *(((char*)ptr) + 1);
		*(((char*)(&val)) + 2) = *(((char*)ptr) + 2);
		*(((char*)(&val)) + 3) = *(((char*)ptr) + 3);

		return val;
	}
	static inline lp_uint16 arv_guint16_from_unaligned_le_ptr(const char *ptr, lp_int16 offset)
	{
		lp_uint16 val;
		if (ptr == NULL)
			return 0;
		ptr += offset;

		*((char*)(&val)) = *((char*)ptr);
		*(((char*)(&val)) + 1) = *(((char*)ptr) + 1);

		return val;
	}
private:
	const lp_uint8 *m_pbuffer;
	size_t m_buffer_size;
	std::vector<ZipFile*> m_pZipfiles;
	size_t m_header_size;
	ptrdiff_t m_directory_position;
	size_t m_directory_size;
	ptrdiff_t m_directory_offset;
	lp_uint32 n_files;
};

#endif
