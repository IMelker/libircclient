project(examples)

add_executable(censor censor.cpp)
target_link_libraries(censor irc::client)

add_executable(colors colors.cpp)
target_link_libraries(colors irc::client)

add_executable(ircftp censor.cpp)
target_link_libraries(ircftp irc::client)

add_executable(irctest censor.cpp)
target_link_libraries(irctest irc::client)

add_executable(spammer censor.cpp)
target_link_libraries(spammer irc::client)