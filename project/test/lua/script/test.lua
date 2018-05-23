package.path = "E:/github/Lengineset/project/test/lua/script/?.lua;"..package.path
local l_debug = require("ldebug")
function foo4()
	local a = 9
	local b = 10
end

function foo()
	-- print("foo")
	local a = 9
	local b = 10
	foo4()
end

function foo1(n)
	n = n or 0
	foo()
	-- print("foo1")
	local a = 9
	if n == 1 then
		-- print("n = ", n)
		local a = 9
		return 0
	end
	
	return 1
end

function foo2()
	-- print("foo2")
	local a = 9
	foo1(1)
end

function foo3()
	-- print("foo3")
	local a = 9
end

l_debug:set_prefix("E:\\github\\Lengineset\\project\\test\\lua");
l_debug:add_break_point("script/test.lua:22");

foo2()
foo3()
l_debug:show_break_point();

l_debug:release();

