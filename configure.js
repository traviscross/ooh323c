
function boolToStr(opt){
   if (opt == false)   return "no";
   else if (opt == true)   return "yes";
   return "*** undefined ***";
}

function strToBool(opt)
{
   if (opt == 0 || opt == "no")   return false;
   else if (opt == 1 || opt == "yes")   return true;
   return false;
}

function usage(){
   var txt;
   txt = "USAGE:\n";
   txt += "\tcscript " + WScript.ScriptName + " [options]\n";
   txt += "\tcscript " + WScript.ScriptName + " help\n\n";
   txt += "Options can be specified in the form <option>=<value>\n\n";
   txt += "\nCongigure options, default value given in parentheses:\n\n";
   txt += "\t--prefix=<dir>      - Directory in which the libraries and executables\n"
   txt += "\t                      should be installed(current directory)\n";
   txt += "\t--objs=<dir>        - Directory where intermediate files will be kept(.\OBJECTS)\n";
   txt += "\t--media=<yes/no>    - Do you want to install media library?(yes)\n";
   txt += "\t--examples=<yes/no> - Do you want to install example applications?(yes)\n";
   WScript.Echo(txt);
}


function createConfigFile()
{
   var fso, cf;
   var configFile = "config.msvc";
   fso = new ActiveXObject("Scripting.FileSystemObject");

   cf = fso.CreateTextFile(configFile, true);
   cf.WriteLine("# " + configFile);
   cf.WriteLine("# This file is generated automatically by " + WScript.ScriptName + ".");
   cf.WriteBlankLines(1);
   cf.WriteLine("INSTALL_ROOT="+installDir);
   cf.WriteLine("MEDIA_INSTALL="+mediaInstall);
   cf.WriteLine("EXAMPLES_INSTALL="+examplesInstall);
   cf.WriteLine("INSTALL_HEADERS="+headersInstall);
   cf.WriteLine("INTDIR="+objDir);
   cf.close();

}


/* main() - Execution beigns here */
var installDir=".";
var mediaInstall= "yes";
var examplesInstall = "yes";
var headersInstall = "no";
var version = "0.7";
var error = 0;
var objDir="./OBJECTS";

for (i = 0; (i < WScript.Arguments.length) && (error == 0); i++)
{
   var arg, opt;
   arg = WScript.Arguments(i);
   opt = arg.substring(0, arg.indexOf("="));
   if (opt.length == 0)
      opt = arg.substring(0, arg.indexOf(":"));
   if (opt.length > 0)
   {
      if (opt == "--prefix")  
         installDir = arg.substring(opt.length + 1, arg.length);
      else if (opt == "--media")  
         mediaInstall = arg.substring(opt.length + 1, arg.length);
      else if (opt == "--examples")
         examplesInstall = arg.substring(opt.length + 1,arg.length);
      else if (opt == "--objs")
         objDir = arg.substring(opt.length + 1,arg.length);
      else if (opt == "--headers")
         headersInstall = arg.substring(opt.length + 1,arg.length);
      else if (opt == "--help")
          {
         usage();
         WScript.Quit(0);
      }else
         error++;
   }
}

if (error != 0) {
    WScript.Echo("Invalid parameters passed to configure script\n")
        usage();
        WScript.Quit(error);
}

var txtOut = "\nObjective Open H.323 Stack Configuration..\n";
txtOut += "-------------------------------------------\n";
txtOut += "\tInstall Dir: " + installDir + "\n";
txtOut += "\tInstall Media Library: " + mediaInstall + "\n";
txtOut += "\tInstall Examples: " + examplesInstall + "\n";
txtOut += "\tObjects Dir: " + objDir + "\n";
txtOut += "\n\nGenerating configuration file...\n";

WScript.Echo(txtOut);
createConfigFile();