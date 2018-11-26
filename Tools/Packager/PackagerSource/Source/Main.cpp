#include <bjs/brew.js>

int main()
{
    bjs::js::NativeContext contx = bjs::js::CreateContext();
    bjs::Environment env(contx, true);
    bjs::EvaluationResult rest = env.EvaluateFile("romfs:/index.js");
    if(rest.IsSuccess()) return 0;
    return -1;
}