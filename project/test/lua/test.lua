local glib = require("glib");
print(1111, glib);

print(tonumber(glib.add(1, 3)));

print(tonumber("4"));


function subtest(a, b)
    -- body
    print(999, a, b);
    return a - b;
end


