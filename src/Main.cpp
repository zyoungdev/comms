#include "src/Engine.hpp"

/*
 * Include Heirarchy
 * -----------------
 *
 * Include
 *  ^- Macros
 *     ^- Alias
 *        ^- Helper
 *           ^- | Data
 *                ^- Global
 *                   ^- Exec
 *                      ^- Taglist
 *                       ^-  | Buffer
 *                           | Event
 *                           | Font
 *                           | Prog_Win
 *                           | Render
 *                           | Socket
 *                             ^- components/Include
 *                                ^- | Command
 *                                   | Program
 *                                   | Window
 *                                     ^- commands/Include
 *                                        ^- Engine
 *                                           ^- main
 *
 *
 */

/*
 * Program Flow
 * ------------
 *                                                                         __.-> Exec     -> Command Functors
 *                                                                       /`__.-> Try_Exec -> Command Functors
 * Engine<Init> -> Engine<Main_Loop> -> Engine<Parse_Args> -> Command<Run>`
 *       \                          `-> Engine<Parse_Args> -> Command<Run> -> Try_Exec -> Command Functors
 *        \                                                               `-> Exec     -> Command Functors
 *         \
 *          `Engine<Net_Loop> -> Engine<Parse_Buffer> -> Command<Run> -> Net Comm Map -> Exec    -> Command Functors
 *                                                                                  `-> Try_Exec -> Command Functors
 *                                                   
 */

int main( int argc, char* argv[] )
{
  using namespace Comms;

  struct Cmd
  {
    const char* flag;
    bool kill = false;
    void(*fn)(void);
  };

  auto cmds = Vec<Cmd>{
    { "-s", true, [](){ fct::print("hello"); Engine<Server,Init>{}; } }
  };

  for ( auto const& cmd : cmds )
  {
    for ( int i = 0 ; i < argc ; i++ )
    {
      if (    strlen( argv[i] ) == strlen( cmd.flag )
           && strncmp( argv[i], cmd.flag, strlen( cmd.flag ) ) == 0 )
      {
        (*cmd.fn)();

        if ( cmd.kill )
          goto end_exec;
      }
    }
  }

  end_exec: ; // end execution early
}
