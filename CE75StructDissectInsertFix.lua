-- Author: Dark Byte
-- CE 7.5 Structure Dissect insert element error fix
-- https://forum.cheatengine.org/viewtopic.php?p=5783154#5783154
-- Sometimes when you insert a non-pointer element into a structure you'll get an error
-- This script will prevent that error.
-- It's fixed in Patreon, and eventually on main as well, but this is an easy fix to put in your autorun. The fix does the same.
--
-- Usage:
--  Automatic; just copy this LUA file into CE's autorun folder

function forEachAndFutureForm(classname, func)
  local i
  for i=0,getFormCount()-1 do
    local f
    f=getForm(i)
    if f.ClassName==classname then
      func(f)
    end
  end

  registerFormAddNotification(function(f)
    if classname==f.ClassName then
      f.registerFirstShowCallback(function()
        func(f)
      end)
    end
  end)
end

forEachAndFutureForm('TfrmStructures2ElementInfo',function(f)
  f.cbStructType.ItemIndex=0
end)
