# OPENSROUCE CODE DEMO TO WORK LIKES THE WINDOWS TOOLS EVERYTHING

## What Win32 APIs do we need?

| API                                                          | Description                                                  |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| [FSCTL_GET_NTFS_VOLUME_DATA](https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_get_ntfs_volume_data) | Retrieves information about the specified NTFS file system volume. |
| [FSCTL_CREATE_USN_JOURNAL](https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_create_usn_journal) | Creates an update sequence number (USN) change journal stream on a target volume, or modifies an existing change journal stream. |
| [FSCTL_DELETE_USN_JOURNAL](https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_delete_usn_journal) | Deletes the update sequence number (USN) change journal on a volume, or waits for notification of change journal deletion. |
| [FSCTL_QUERY_USN_JOURNAL](https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_query_usn_journal) | Queries for information on the current update sequence number (USN) change journal, its records, and its capacity. |
| [FSCTL_READ_USN_JOURNAL](https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_read_usn_journal) | Retrieves the set of update sequence number (USN) change journal records between two specified USN values. |
| [FSCTL_ENUM_USN_DATA](https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_enum_usn_data) | Enumerates the update sequence number (USN) data between two specified boundaries to obtain master file table (MFT) records. |
| [FSCTL_FIND_FILES_BY_SID](https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_find_files_by_sid) | Searches a directory for a file whose creator owner matches the specified SID. |
| [FSCTL_GET_NTFS_FILE_RECORD](https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_get_ntfs_file_record) | Retrieves the first file record that is in use and is of a lesser than or equal ordinal value to the requested file reference number. |
| [FSCTL_READ_FILE_USN_DATA](https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_read_file_usn_data) | Retrieves the update sequence number (USN) change-journal information for the specified file or directory. |
| [FSCTL_USN_TRACK_MODIFIED_RANGES](https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_usn_track_modified_ranges) | Enables range tracking feature for update sequence number (USN) change journal stream on a target volume, or modifies already enabled range tracking parameters. |
| [FSCTL_WRITE_USN_CLOSE_RECORD](https://docs.microsoft.com/en-us/windows/win32/api/winioctl/ni-winioctl-fsctl_write_usn_close_record) | Generates a record in the update sequence number (USN) change journal stream for the input file. |
