using System;
using System.IO;
using System.Collections.Generic;
using System.Diagnostics;

class Entrypoint {
    static int Main() {
        string[] args = Environment.GetCommandLineArgs();

        if (args.Length < 2) {
            Console.Error.WriteLine(@"USAGE

  ape   PROG [ARGV1,ARGV2,...]
  ape - PROG [ARGV0,ARGV1,...]");
            return 1;
        }

        // TODO: support rewriting argument zero
        if (args[1] == "-") {
            Console.Error.WriteLine("`-` unsupported");
            return 2;
        }

        // Figure out the arguments
        uint index = 1;
        string fileName = Path.GetFullPath(args[index]);
        string arguments = "";
        for (uint i = index + 1; i < args.Length; i++) {
            arguments += string.Format(" \"{0}\"", args[i].Replace("\"", "\\\""));
        }

        // Run the binary
        using(Process process = new Process()) {
            process.StartInfo.UseShellExecute = false;
            process.StartInfo.FileName = fileName;
            process.StartInfo.Arguments = arguments;
            process.Start();
            process.WaitForExit();
            return process.ExitCode;
        }
    }
}