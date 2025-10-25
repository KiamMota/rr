#ifndef _RRFILE_HPP
#define _RRFILE_HPP


enum class file_type {
    Regular,     // Arquivo normal
    Directory,   // Pasta
    Symlink,     // Link simbÃ³lico
    Character,   // Arquivo de dispositivo de caractere
    Block,       // Arquivo de dispositivo em bloco
    FIFO,        // Named pipe
    Socket,      // Socket
    Unknown      // Tipo desconhecido / nÃ£o suportado
};
#endif
