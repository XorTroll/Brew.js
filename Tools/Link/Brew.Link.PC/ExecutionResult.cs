using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Brew.Link.PC
{
    // Note that this is a simple, C# adapted clone of Brew::API::Type enum from Brew.js C++ API
    public enum Type
    {
        None,
        Undefined,
        Null,
        Boolean,
        Number,
        String,
        ObjectArray,
        Buffer,
        Pointer
    }

    // Note that this is a simple, C# adapted clone of Brew::ExecutionResult struct from Brew.js C++ API
    public struct ExecutionResult
    {
        public bool IsOK { get; set; }
        public string Result { get; set; }
        public int NativeCode { get; set; }
        public Type Type { get; set; }

        // Encoded Brew.js result: "<OK>|<Result>|<Code>|<Type>", example: "true|undefined|0|Undefined"
        public static ExecutionResult decodeFromString(string Encoded)
        {
            ExecutionResult res = new ExecutionResult();
            string[] parts = Encoded.Split('|');
            if(parts.Length != 4) Environment.Exit(-1);
            res.IsOK = bool.Parse(parts[0]);
            res.Result = parts[1];
            res.NativeCode = int.Parse(parts[2]);
            Enum.TryParse(parts[3], out Type ptype);
            res.Type = ptype;
            return res;
        }

        public override string ToString()
        {
            return Result;
        }
    }
}
