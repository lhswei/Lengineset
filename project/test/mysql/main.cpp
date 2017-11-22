
// #include <stdio.h>
// #include <stdlib.h>
// #include <iostream>

// #include <mysql_common.h>
// #include <mysql_devapi.h>
// #include <mysql_xapi.h>
// // #include <devapi/document.h>

// using ::std::cout;
// using ::std::endl;

// int main(int argc, char *argv[])
// {
//     try
//     {
//         cout << "start connect..." << endl;
//         mysqlx::Session from_options("10.20.79.112", 3306, "myroot ", "1234", "test");
//         cout << "start connect success" << endl;
//         mysqlx::Schema sche = from_options.getDefaultSchema();
//         mysqlx::Collection coll = sche.getCollection("machine");
//         mysqlx::DocResult docs = coll.find("ID = 966").execute();
//         mysqlx::DbDoc doc = docs.fetchOne();

//         for (int i = 0; doc; ++i, doc = docs.fetchOne())
//         {
//             cout << "doc#" << i << ": " << doc << endl;
//             for (mysqlx::Field fld : doc)
//             {
//                 cout << " field `" << fld << "`: " << doc[fld] << endl;
//             }
//         }
//     }
//     catch (const mysqlx::Error &err)
//     {
//         cout << "ERROR: " << err << endl;
//         return 1;
//     }
//     catch (std::exception &ex)
//     {
//         cout << "STD EXCEPTION: " << ex.what() << endl;
//         return 1;
//     }
//     catch (const char *ex)
//     {
//         cout << "EXCEPTION: " << ex << endl;
//         return 1;
//     }

//     return 0;
// }

#include <iostream>
#include <mysql_devapi.h>

using ::std::cout;
using ::std::endl;
using namespace ::mysqlx;

int main(int argc, const char *argv[]) try
{

    const char *phost = (argc > 1 ? argv[1] : "10.20.79.112");
    unsigned short port = (argc > 2 ? atoi(argv[2]) : 0);
    const char *user = (argc > 3 ? argv[3] : "myroot");
    const char *pwd = (argc > 4 ? argv[4] : "1234");

    if (0 == port)
    {
        // Default MySQL X port
        port = 33060;
    }

    cout << "Creating session on ip: " << phost << " port: " << port
         << " user: "<< user << " pwd: "<< pwd <<" ..." << endl;

    Session sess(phost, port, user, pwd);

    cout << "Session accepted, creating collection..." << endl;

    Schema sch = sess.getSchema("test");
    Collection coll = sch.createCollection("c1", true);

    cout << "Inserting documents..." << endl;

    coll.remove("true").execute();

    {
        Result add;

        add = coll.add(R"({ "name": "foo", "age": 1 })").execute();
        cout << "- added doc with id: " << add.getDocumentId() << endl;

        add = coll.add(R"({ "name": "bar", "age": 2, "toys": [ "car", "ball" ] })")
                  .execute();
        cout << "- added doc with id: " << add.getDocumentId() << endl;

        add = coll.add(R"({
       "name": "baz",
        "age": 3,
       "date": { "day": 20, "month": "Apr" }
    })")
                  .execute();
        cout << "- added doc with id: " << add.getDocumentId() << endl;

        add = coll.add(R"({ "_id": "myuuid-1", "name": "foo", "age": 7 })")
                  .execute();
        cout << "- added doc with id: " << add.getDocumentId() << endl;
    }

    cout << "Fetching documents..." << endl;

    DocResult docs = coll.find("age > 1 and name like 'ba%'").execute();

    DbDoc doc = docs.fetchOne();

    for (int i = 0; doc; ++i, doc = docs.fetchOne())
    {
        cout << "doc#" << i << ": " << doc << endl;

        for (Field fld : doc)
        {
            cout << " field `" << fld << "`: " << doc[fld] << endl;
        }

        string name = doc["name"];
        cout << " name: " << name << endl;

        if (doc.hasField("date") && Value::DOCUMENT == doc.fieldType("date"))
        {
            cout << "- date field" << endl;
            DbDoc date = doc["date"];
            for (Field fld : date)
            {
                cout << "  date `" << fld << "`: " << date[fld] << endl;
            }
            string month = doc["date"]["month"];
            int day = date["day"];
            cout << "  month: " << month << endl;
            cout << "  day: " << day << endl;
        }

        if (doc.hasField("toys") && Value::ARRAY == doc.fieldType("toys"))
        {
            cout << "- toys:" << endl;
            for (auto toy : doc["toys"])
            {
                cout << "  " << toy << endl;
            }
        }

        cout << endl;
    }

    cout << "Done!" << endl;
}
catch (const mysqlx::Error &err)
{
    cout << "ERROR: " << err << endl;
    return 1;
}
catch (std::exception &ex)
{
    cout << "STD EXCEPTION: " << ex.what() << endl;
    return 1;
}
catch (const char *ex)
{
    cout << "EXCEPTION: " << ex << endl;
    return 1;
}
