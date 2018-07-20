#include "test.h"
#include "../../dna_c_binding/include/dna_c_binding.h"

#include <stdio.h>

void TestHcDna::serializeAndDeserialize() {
  Dna *dna;
  Dna *dna2;
  char *buf;

  dna = holochain_dna_create();
  buf = holochain_dna_to_json(dna);
  holochain_dna_free(dna);

  dna2 = holochain_dna_create_from_json(buf);
  holochain_dna_string_free(buf);

  buf = holochain_dna_get_dna_spec_version(dna2);

  QCOMPARE(QString("2.0"), QString(buf));

  holochain_dna_string_free(buf);
  holochain_dna_free(dna2);
}

void TestHcDna::canGetName() {
  Dna *dna = holochain_dna_create_from_json("{\"name\":\"test\"}");
  char *buf = holochain_dna_get_name(dna);

  QCOMPARE(QString("test"), QString(buf));

  holochain_dna_string_free(buf);
  holochain_dna_free(dna);
}

void TestHcDna::canSetName() {
  Dna *dna = holochain_dna_create();

  holochain_dna_set_name(dna, "test");

  char *buf = holochain_dna_get_name(dna);

  QCOMPARE(QString("test"), QString(buf));

  holochain_dna_string_free(buf);
  holochain_dna_free(dna);
}

void TestHcDna::canGetZomeNames() {
  Dna *dna = holochain_dna_create_from_json("{\"name\":\"test\","
                                            "\"zomes\":["
                                            "{\"name\":\"zome1\",\"description\":\"lorem\",\"config\":{},\"entry_types\":[],\"capabilities\":[]},"
                                            "{\"name\":\"zome2\",\"description\":\"lorem\",\"config\":{},\"entry_types\":[],\"capabilities\":[]}"
                                            "]}");
  QVERIFY(dna != 0);

  CStringVec names;
  holochain_dna_get_zome_names(dna, &names);
  QCOMPARE(names.len, (size_t) 2);

  QString name1 = QString("%1").arg(names.ptr[0]);
  QString name2 = QString("%1").arg(names.ptr[1]);

  QCOMPARE(name1, QString("zome1"));
  QCOMPARE(name2, QString("zome2"));

  holochain_dna_free_zome_names(&names);
  holochain_dna_free(dna);
}

void TestHcDna::canGetCapabilityNames() {
    Dna *dna = holochain_dna_create_from_json("{\"name\":\"test\","
                                              "\"zomes\":["
                                              "{\"name\":\"zome1\",\"description\":\"lorem\",\"config\":{},\"capabilities\":["
                                              "{"
                                              "    \"name\": \"test_cap\","
                                              "            \"capability\": {"
                                              "        \"membrane\": \"public\""
                                              "    },"
                                              "    \"fn_declarations\": ["
                                              "    {"
                                              "        \"name\": \"main\","
                                              "        \"signature\": {"
                                              "            \"inputs\": [],"
                                              "            \"outputs\": []"
                                              "        }"
                                              "    }"
                                              "    ],"
                                              "    \"code\": {"
                                              "        \"code\": \"AGFzbQEAAAABBQFgAAF/AwIBAAUDAQARBxECBG1haW4AAAZtZW1vcnkCAAoHAQUAQbkKCw==\""
                                              "    }"
                                              "},"
                                              "{"
                                              "    \"name\": \"test_cap2\","
                                              "            \"capability\": {"
                                              "        \"membrane\": \"public\""
                                              "    },"
                                              "    \"fn_declarations\": ["
                                              "    {"
                                              "        \"name\": \"main\","
                                              "        \"signature\": {"
                                              "            \"inputs\": [],"
                                              "            \"outputs\": []"
                                              "        }"
                                              "    }"
                                              "    ],"
                                              "    \"code\": {"
                                              "        \"code\": \"AGFzbQEAAAABBQFgAAF/AwIBAAUDAQARBxECBG1haW4AAAZtZW1vcnkCAAoHAQUAQbkKCw==\""
                                              "    }"
                                              "}"
                                              "]}"
                                              "]}");
    QVERIFY(dna != 0);

    CStringVec names;
    holochain_dna_get_capabilities_names(dna, "zome1", &names);
    QCOMPARE(names.len, (size_t) 2);

    QString name1 = QString("%1").arg(names.ptr[0]);
    QString name2 = QString("%1").arg(names.ptr[1]);

    QCOMPARE(name1, QString("test_cap"));
    QCOMPARE(name2, QString("test_cap2"));

    holochain_dna_free_zome_names(&names);
    holochain_dna_free(dna);
}



void TestHcDna::canGetFunctionNames() {
    Dna *dna = holochain_dna_create_from_json("{\"name\":\"test\","
                                              "\"zomes\":["
                                              "{\"name\":\"zome1\",\"description\":\"lorem\",\"config\":{},\"capabilities\":["
                                              "{"
                                              "    \"name\": \"test_cap\","
                                              "            \"capability\": {"
                                              "        \"membrane\": \"public\""
                                              "    },"
                                              "    \"fn_declarations\": ["
                                              "    {"
                                              "        \"name\": \"main\","
                                              "        \"signature\": {"
                                              "            \"inputs\": ["
                                              "                {"
                                              "                    \"name\": \"param1\","
                                              "                    \"type\": \"string\""
                                              "                }"
                                              "            ],"
                                              "            \"outputs\": []"
                                              "        }"
                                              "    },"
                                              "    {"
                                              "        \"name\": \"test\","
                                              "        \"signature\": {"
                                              "            \"inputs\": [],"
                                              "            \"outputs\": []"
                                              "        }"
                                              "    }"
                                              "    ],"
                                              "    \"code\": {"
                                              "        \"code\": \"AGFzbQEAAAABBQFgAAF/AwIBAAUDAQARBxECBG1haW4AAAZtZW1vcnkCAAoHAQUAQbkKCw==\""
                                              "    }"
                                              "}"
                                              "]}"
                                              "]}");
    QVERIFY(dna != 0);

    CStringVec names;
    holochain_dna_get_function_names(dna, "zome1", "test_cap", &names);
    QCOMPARE(names.len, (size_t) 2);

    QString name1 = QString("%1").arg(names.ptr[0]);
    QString name2 = QString("%1").arg(names.ptr[1]);

    QCOMPARE(name1, QString("main"));
    QCOMPARE(name2, QString("test"));

    holochain_dna_free_zome_names(&names);
    holochain_dna_free(dna);
}

void TestHcDna::canGetFunctionParameters() {
    Dna *dna = holochain_dna_create_from_json("{\"name\":\"test\","
                                              "\"zomes\":["
                                              "{\"name\":\"zome1\",\"description\":\"lorem\",\"config\":{},\"capabilities\":["
                                              "{"
                                              "    \"name\": \"test_cap\","
                                              "            \"capability\": {"
                                              "        \"membrane\": \"public\""
                                              "    },"
                                              "    \"fn_declarations\": ["
                                              "    {"
                                              "        \"name\": \"main\","
                                              "        \"signature\": {"
                                              "            \"inputs\": ["
                                              "                {"
                                              "                    \"name\": \"param1\","
                                              "                    \"type\": \"string\""
                                              "                }"
                                              "            ],"
                                              "            \"outputs\": []"
                                              "        }"
                                              "    },"
                                              "    {"
                                              "        \"name\": \"test\","
                                              "        \"signature\": {"
                                              "            \"inputs\": [],"
                                              "            \"outputs\": []"
                                              "        }"
                                              "    }"
                                              "    ],"
                                              "    \"code\": {"
                                              "        \"code\": \"AGFzbQEAAAABBQFgAAF/AwIBAAUDAQARBxECBG1haW4AAAZtZW1vcnkCAAoHAQUAQbkKCw==\""
                                              "    }"
                                              "}"
                                              "]}"
                                              "]}");
    QVERIFY(dna != 0);

    CStringVec names;
    holochain_dna_get_function_parameters(dna, "zome1", "test_cap", "main", &names);
    QCOMPARE(names.len, (size_t) 1);

    QString name1 = QString("%1").arg(names.ptr[0]);
    QCOMPARE(name1, QString("param1"));

    holochain_dna_free_zome_names(&names);

    holochain_dna_get_function_parameters(dna, "zome1", "test_cap", "test", &names);
    QCOMPARE(names.len, (size_t) 0);

    holochain_dna_free_zome_names(&names);

    holochain_dna_free(dna);
}


QTEST_MAIN(TestHcDna)
