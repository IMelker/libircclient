# libircclient
libircclient is a small but powerful library, which implements client-server IRC protocol. It has all features needed to create your own IRC client or bot, including multi-threading support, sync and async interfaces, CTCP/DCC support, colors, SSL connections and so on.

## Features
- Full coverage of the IRC protocol using providing functions
- Supports multiple simultaneous connections
- Supports both plain and SSL connections with the optional certificate check
- Comprehensive documentation, examples and FAQ
- Full multi-threading support, the library is thread-safe
- All the processing could be handled by a single thread even if multiple connections are used
- Non-blocking, asynchronous event-based interface implemented with callbacks
- Extra support for the socket-based applications, which use select()
- CTCP support with optional build-in reply code
- Supports CTCP PING necessary to pass the “spoof check” implemented by most IRC servers
- Flexible DCC support, including both DCC chat, and DCC file transfer
- Can both initiate and react to initiated DCC
- Can accept or decline DCC sessions asynchronously
- Written in plain C, very small binary size
- Compatible with RFC 1459 and most IRC clients
- Free software licensed under LGPL license
- Supports Linux as well as any POSIX-compliant Unix, Mac OS X and Microsoft Windows
- Could be compiled with optional IPv6 support
- Cocoa interface by Nathan Ollerenshaw

## Building from source

The library uses autoconf so it is build in a fairly typical way on most Unix systems:

```
./configure
make
```
This will build both the library and various examples in the examples subdirectory.

Configure script also accepts parameters, optional useful parameters accepted are `–enable-openssl` and `–enable-ipv6` which correspondingly enable the SSL and IPv6 connectivity. Use `–enable-shared` to build a shared library.

The same procedure is used to build the Win32 binary using the MinGW compiler.

For the rest see the Doxygen documentation in the header files and http://www.ulduzsoft.com/libircclient/

