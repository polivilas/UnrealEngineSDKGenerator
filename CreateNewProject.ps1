Write-Host "Create $($args[1]) from $($args[0])"

$source = @"
using System.IO;

namespace UnrealEngineSDKGenerator
{
	public class Program
	{
		public static void Create(string oldname, string newname)
		{
			DirectoryInfo newdir = new DirectoryInfo(Path.Combine("./Target", newname));
			newdir.Create();
			CopyFilesRecursively(new DirectoryInfo(Path.Combine("./Target", oldname)), newdir);

			File.WriteAllText(newname + ".vcxproj", File.ReadAllText(oldname + ".vcxproj").Replace(oldname, newname));
			File.WriteAllText(newname + ".vcxproj.filters", File.ReadAllText(oldname + ".vcxproj.filters").Replace(oldname, newname));
		}

		public static void CopyFilesRecursively(DirectoryInfo source, DirectoryInfo target)
		{
			foreach (DirectoryInfo dir in source.GetDirectories())
				CopyFilesRecursively(dir, target.CreateSubdirectory(dir.Name));
			foreach (FileInfo file in source.GetFiles())
				file.CopyTo(Path.Combine(target.FullName, file.Name));
		}
	}
}
"@

Add-Type -TypeDefinition $Source -Language CSharp

[UnrealEngineSDKGenerator.Program]::Create($args[0], $args[1])