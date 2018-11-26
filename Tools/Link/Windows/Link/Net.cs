using System;

namespace bjs.net
{
    public static class EvaluationResultParser
    {
        public static EvaluationResult Parse(string Encoded)
        {
            EvaluationResult res = new EvaluationResult();
            string[] parts = Encoded.Split('|');
            if (parts.Length != 4) Environment.Exit(-1);
            res.IsOK = bool.Parse(parts[0]);
            res.Result = parts[1];
            res.NativeCode = int.Parse(parts[2]);
            Enum.TryParse(parts[3], out Type ptype);
            res.Type = ptype;
            return res;
        }
    }

    public class EvaluationResult
    {
        public bool IsOK { get; set; }
        public string Result { get; set; }
        public int NativeCode { get; set; }
        public Type Type { get; set; }

        public override string ToString()
        {
            string ress = "Evaluation: { ";
            ress += " Result = " + Type.ToString() + ": \"" + Result + "\"";
            if(!IsOK) ress += ", OK = Failure: { ErrorCode = " + NativeCode.ToString() + " }";
            else ress += ", OK = Success";
            ress += " }";
            return ress;
        }
    }
}