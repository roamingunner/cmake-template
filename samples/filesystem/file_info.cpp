#include <iostream> 
#include <string> 
#include <functional>
#include <cstring> 

// -- Unix-specific Headers -------//
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <pwd.h>
#include <grp.h>


std::string 
get_descriptor_type(struct stat const& fs)
{
    int ftype = fs.st_mode & S_IFMT;
    if(ftype == S_IFDIR)  return "Directory";
    if(ftype == S_IFREG)  return "Regular file";
    if(ftype == S_IFLNK)  return "Symbolic link";
    if(ftype == S_IFBLK)  return "Block device file";
    if(ftype == S_IFCHR)  return "Character device file";    
    if(ftype == S_IFIFO)  return "FIFO or pipe"; 
    if(ftype == S_IFSOCK) return "Socket";
    return "Unknown";    
}; 

int main(int argc, char** argv)
{
    if(argc != 2) {
        std::printf(" Usage: %s <FILE> \n", argv[0]);
        return 0;
    }

    struct stat fs; 
    struct passwd* fs_user = nullptr; 
    struct group*  fs_group = nullptr;


    //int fd = open(argv[1], O_RDONLY);

    if( ::stat(argv[1], &fs ) == -1) {
        std::fprintf(stderr, "Error: unable to open file => error = %s \n", strerror(errno) );
        return -1;
    }

    if( ( fs_user = ::getpwuid(fs.st_uid)) == nullptr )
    {
        std::fprintf(stderr, " Error: unable to get owner information \n");
        return -1;
    }

    if( ( fs_group = ::getgrgid(fs.st_gid)) == nullptr )
    {
        std::fprintf(stderr, " Error: unable to get group information \n");
        return -1;
    }


    // Print boolean as 'true' or 'false'
    std::cout << std::boolalpha;
    std::cout << "  [*]  File size in bytes: " << fs.st_size << '\n';
    std::cout << "  [*]  File size in Kilo bytes: " << static_cast<double>(fs.st_size) / 1024 << '\n';
    std::cout << "  [*]  File size in Mega bytes: " << static_cast<double>(fs.st_size) / (1024 * 1024) << '\n';
    std::cout << "  [*]           File type: " << get_descriptor_type(fs) << '\n';

    std::cout << "\n TEST FILE TYPE \n";
    std::cout << "  [*] ?      Is regular file =>> " << static_cast<bool>( S_ISREG(fs.st_mode) ) << '\n';
    std::cout << "  [*] ?         Is directory =>> " << static_cast<bool>( S_ISDIR(fs.st_mode) ) << '\n';
    std::cout << "  [*] ?     Is symbolic link =>> " << static_cast<bool>( S_ISLNK(fs.st_mode) ) << '\n';
    std::cout << "  [*] ?     Is block device  =>> " << static_cast<bool>( S_ISBLK(fs.st_mode) ) << '\n';
    std::cout << "  [*] ? Is character device  =>> " << static_cast<bool>( S_ISCHR(fs.st_mode) ) << '\n';

    std::cout << "\n ACCESS TIME \n";
    std::cout << "  [*] Time =>   last change: " << ctime(&fs.st_ctime);
    std::cout << "  [*] Time =>   last access: " << ctime(&fs.st_atime);
    std::cout << "  [*] Time => last modified: " << ctime(&fs.st_mtime);

    std::cout <<  "\n OWNER and GROUP \n";
    std::cout << "  [*] Owner: uid = " << fs.st_uid << " ;  user = " << fs_user->pw_name  << '\n';
    std::cout << "  [*] Group: gid = " << fs.st_gid << " ; group = " << fs_group->gr_name  << '\n';

    std::cout << "\n FILE PERMISSIONS \n";
    std::cout << "  =>>  Stick Bits: " << (fs.st_mode & S_ISUID ? "suid" : "-") 
              << "  "                  << (fs.st_mode & S_ISGID ? "sgid" : "-")
              << '\n';
    std::cout << "  =>>  Owner: " << (fs.st_mode & S_IRUSR  ? "r" : "-")   // File is readable   (can be read)
              << " "         << (fs.st_mode & S_IWUSR  ? "w" : "-")        // File is writeable  (can be written)
              << " "         << (fs.st_mode & S_IXUSR  ? "x" : "-")        // File is executable (can be executed - execv syscall)
              << '\n';
    std::cout << "  =>>  Group: " << (fs.st_mode & S_IRGRP  ? "r" : "-")  
              << " "            << (fs.st_mode & S_IWGRP  ? "w" : "-")  
              << " "            << (fs.st_mode & S_IXGRP  ? "x" : "-")
              << '\n';
    std::cout << "  =>> Others: " << (fs.st_mode & S_IROTH  ? "r" : "-")  
              << " "             << (fs.st_mode & S_IWOTH  ? "w" : "-")  
              << " "             << (fs.st_mode & S_IXOTH  ? "x" : "-")
              << '\n';    
    std::cout << " \n- Note: (r - read) ; (w - write); (x - execute) ; (suid - set-UID bit set) ; (sgid - set-GID bit set) \n";

    return 0;
}