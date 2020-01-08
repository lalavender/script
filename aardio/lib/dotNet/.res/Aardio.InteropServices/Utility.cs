using System;
using System.CodeDom.Compiler;
using System.Collections;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Aardio.InteropServices
{
	[ClassInterface(ClassInterfaceType.AutoDispatch), ComVisible(true)]
	public class Utility
	{
		public CCodeCompiler CreateCompiler(string provideType)
		{
			object obj = this.loadAssembly("System").CreateInstance(provideType);
			if (obj == null)
			{
				return null;
			}
			return new CCodeCompiler(obj as CodeDomProvider);
		}

		public object InvokeMember(object assemblyName, string typeName, string methodName, int invokeAttr, object args, object target)
		{
			Assembly assembly = assemblyName as Assembly;
			if (assembly == null)
			{
				assembly = this.loadAssembly(assemblyName as string);
			}
			if (assembly != null)
			{
				return assembly.GetType(typeName).InvokeMember(methodName, (BindingFlags)invokeAttr, null, target, (args as ArrayList).ToArray());
			}
			return null;
		}

		public object InvokeObjectMember(object target, string methodName, int invokeAttr, object args)
		{
			if (target == null)
			{
				return null;
			}
			return target.GetType().InvokeMember(methodName, (BindingFlags)invokeAttr, null, target, (args as ArrayList).ToArray());
		}

		public Assembly loadAssembly(string assemblyName)
		{
			try
			{
				Assembly assembly = Assembly.LoadWithPartialName(assemblyName);
				Assembly result = assembly;
				return result;
			}
			catch (SystemException)
			{
			}
			try
			{
				Assembly assembly2 = Assembly.Load(AssemblyName.GetAssemblyName(assemblyName));
				Assembly result = assembly2;
				return result;
			}
			catch (SystemException)
			{
			}
			try
			{
				Assembly assembly3 = Assembly.LoadFrom(assemblyName);
				Assembly result = assembly3;
				return result;
			}
			catch (SystemException)
			{
			}
			try
			{
				Assembly assembly4 = Assembly.LoadFile(assemblyName);
				Assembly result = assembly4;
				return result;
			}
			catch (SystemException)
			{
			}
			return null;
		}
	}
}
