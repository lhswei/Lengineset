print(111, rpc)
rpc = rpc or {}
print(222, closure)

function rpc_call(cmd, ...)
    -- body
    print(2222, cmd, ...)
    return "hello world!", 100
end

function make_closure(cmd)
    -- body
    if type(cmd) ~= "string" or cmd == "" then
        return;
    end
    
    local tb = rpc
    for w in string.gmatch(cmd, "%a+") do
        -- print(w);
        tb[w] = tb[w] or {};
        tb = tb[w];
    end

    local mt = {}
    mt.__call = function(tb, ...)
        -- body
        return rpc_call(cmd, ...);
    end

    if tb ~= rpc then
        setmetatable(tb, mt);
        return 1;
    end
    return 0;
end

make_closure("online.childrenday.dailyaward");

local msg, code = rpc.online.childrenday.dailyaward("hi!");
print(1111, msg, code)


