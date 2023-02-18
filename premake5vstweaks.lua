require('vstudio')

local vs = premake.vstudio.vc2010

premake.override(premake.vstudio.vc2010.elements, "project", function(base, prj)
    local calls = base(prj)
    --table.insertafter(calls, vs.xmlDeclaration, premakeVersionComment)
    return calls
end)

premake.override(premake.vstudio.vc2010.elements, "globals", function(base, prj)
    local calls = base(prj)
    table.insertafter(calls, vs.globals, premake.w('<ShowAllFiles>true</ShowAllFiles>'))
    return calls
end)