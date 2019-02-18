#include "ArvZip.h"

ArvZip::ArvZip(const void *pbuffer, size_t size):m_pbuffer(NULL)
{
	const void *ptr;
	int i;
	bool directory_found;

	if (pbuffer != NULL && size > 0)
	{
		m_pbuffer = (lp_uint8 *)pbuffer;
		m_buffer_size = size;

		directory_found = false;
		for (i = m_buffer_size - 4; i > 0; i--) {
			if (m_pbuffer[i] == 0x50 &&
				m_pbuffer[i + 1] == 0x4b &&
				m_pbuffer[i + 2] == 0x05 &&
				m_pbuffer[i + 3] == 0x06) {
				m_directory_position = i;
				directory_found = true;
				break;
			}
		}
		if (!directory_found) {
			std::cout << ("[Zip::new] Magic number for end of central directory not found (0x06054b50)");
		}
		else
		{
			ptr = m_pbuffer + m_directory_position;
			n_files = arv_guint16_from_unaligned_le_ptr((char*)ptr, 10);
			if (arv_guint16_from_unaligned_le_ptr((char*)ptr, 8) != n_files) {
				//arv_debug_misc("[Zip::new] Mismatch in number of files");
				n_files = 0;
			}
			else {
				m_directory_size = arv_guint32_from_unaligned_le_ptr((char*)ptr, 12);
				m_directory_offset = arv_guint32_from_unaligned_le_ptr((char*)ptr, 16);
				m_header_size = m_directory_position - (m_directory_offset + m_directory_size);

				//arv_log_misc("[Zip::new] number of files = %d", zip->n_files);
				//arv_log_misc("[Zip::new] directory position = 0x%08x", zip->directory_position);
				//arv_log_misc("[Zip::new] directory size = %d", zip->directory_size);
				//arv_log_misc("[Zip::new] directory offset = 0x%08x", zip->directory_offset);
				//arv_log_misc("[Zip::new] header size = %d", zip->header_size);

				arv_zip_build_file_list();
			}
		}
	}
}

ArvZip::~ArvZip()
{
	while (m_pZipfiles.size())
	{
		ZipFile *pFile = m_pZipfiles.back();
		if (pFile)
			delete pFile;
		m_pZipfiles.pop_back();
	}
}

const std::vector<ZipFile*> ArvZip::arv_zip_get_file_list()
{
	return m_pZipfiles;
}

const char * ArvZip::arv_zip_file_get_name(ZipFile *zip_file)
{
	if (zip_file == NULL)
		return NULL;
	return zip_file->name;
}

size_t ArvZip::arv_zip_file_get_uncompressed_size(ZipFile *zip_file)
{
	if (zip_file == NULL)
		return 0;
	return zip_file->uncompressed_size;
}

void ArvZip::arv_zip_build_file_list()
{
	ZipFile *zip_file;
	const void *ptr;
	ptrdiff_t offset;
	int i;

	offset = m_directory_offset;

	for (i = 0; i < n_files; i++) {
		ptr = m_pbuffer + m_header_size + offset;
		if (arv_guint32_from_unaligned_le_ptr((char*)ptr, 0) != 0x02014b50) {
			std::cout<<("[Zip::build_file_list] Magic number of central directory not found (0x02014b50)");
			//std::cout << ("[Zip::build_file_list] Expected at 0x%08x - found 0x%08x instead",
			//	zip->header_size + offset, ARV_GUINT32_FROM_LE_PTR(ptr, 0));
			return;
		}

		zip_file = new ZipFile;
		zip_file->compressed_size = arv_guint32_from_unaligned_le_ptr((char*)ptr, 20);
		zip_file->uncompressed_size = arv_guint32_from_unaligned_le_ptr((char*)ptr, 24);
		zip_file->offset = arv_guint32_from_unaligned_le_ptr((char*)ptr, 42);
		//zip_file->name = sprintf(((char *)ptr) + 46, arv_guint16_from_unaligned_le_ptr((char*)ptr, 28));
		memcpy(zip_file->name, ((char *)ptr) + 46, arv_guint16_from_unaligned_le_ptr((char*)ptr, 28));
		//arv_log_misc("[Zip::list_files] %s", zip_file->name);

		m_pZipfiles.push_back(zip_file);

		offset += 0x2e +
			arv_guint16_from_unaligned_le_ptr((char*)ptr, 28) + /* filename size */
			arv_guint16_from_unaligned_le_ptr((char*)ptr, 30) + /* extra field */
			arv_guint16_from_unaligned_le_ptr((char*)ptr, 32);  /* file comment */
	}
}

ZipFile *ArvZip::arv_zip_find_file(const char *name)
{
	ZipFile *zip_file;
	for(int nIndex =0;nIndex<m_pZipfiles.size();nIndex++){
		zip_file = m_pZipfiles.at(nIndex);
		if (strcmp(zip_file->name, name) == 0)
			return zip_file;
	}

	return NULL;
}

ptrdiff_t ArvZip::arv_zip_get_file_data(ZipFile *zip_file)
{
	const void *ptr;

	ptr = m_pbuffer + zip_file->offset + m_header_size;

	if (arv_guint32_from_unaligned_le_ptr((char*)ptr, 0) != 0x04034b50) {
		std::cout<<("[Zip::get_file_data] Magic number for file header not found (0x04034b50)")<<std::endl;
		return -1;
	}

	return zip_file->offset + m_header_size +
		arv_guint16_from_unaligned_le_ptr((char*)ptr, 26) +
		arv_guint16_from_unaligned_le_ptr((char*)ptr, 28) + 30;
}

void *ArvZip::arv_zip_get_file(const char *name, size_t *size)
{
	ZipFile *zip_file;
	void *output_buffer;
	ptrdiff_t offset;

	if (size != NULL)
		*size = 0;

	if (name == NULL)
		return NULL;

	zip_file = arv_zip_find_file(name);
	if (!zip_file)
		return NULL;

	offset = arv_zip_get_file_data(zip_file);
	if (offset < 0)
		return NULL;

	output_buffer = malloc(zip_file->uncompressed_size);
	if (output_buffer == NULL)
		return NULL;

	if (zip_file->compressed_size < zip_file->uncompressed_size) {
// 		z_stream zs;
// 		zs.zalloc = NULL;
// 		zs.zfree = NULL;
// 		zs.opaque = NULL;
// 		zs.next_in = (void *)&zip->buffer[offset];
// 		zs.avail_in = zip_file->compressed_size;
// 		zs.next_out = output_buffer;
// 		zs.avail_out = zip_file->uncompressed_size;
// 		inflateInit2(&zs, -MAX_WBITS);
// 		inflate(&zs, Z_FINISH);
// 		inflateEnd(&zs);
	}
	else
		memcpy(output_buffer, m_pbuffer + offset, zip_file->uncompressed_size);

	if (size != NULL)
		*size = zip_file->uncompressed_size;

	return output_buffer;
}