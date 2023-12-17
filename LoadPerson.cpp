struct DBConnectionSettings {
    std::string_view name;
    int connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel log_level;
};

std::optional<DBHandler> ConnectToDataBase(const DBConnectionSettings& connection_settings){

    DBConnector connector(connection_settings.db_allow_exceptions, connection_settings.log_level);
    DBHandler db;
    if (connection_settings.name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(connection_settings.name, connection_settings.connection_timeout);
    } else {
        db = connector.Connect(connection_settings.name, connection_settings.connection_timeout);
    }
    if (!connection_settings.db_allow_exceptions && !db.IsOK()) {
        return std::nullopt;
    }

    return db;
}

struct QueryFilter {
    std::string_view name;
    int min_age;
    int max_age;
}

std::vector<Person> QueryProcessor(const QueryFilter filter, DBHandler& db){
    using namespace std::literals;
    std::ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << filter.min_age << " and "s << filter.max_age << " "s
              << "and Name like '%"s << db.Quote(filter.name) << "%'"s;
    DBQuery query(query_str.str());

    std::vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}


vector<Person> LoadPersons(const DBConnectionSettings& connection_settings, const QueryFilter filter) {
    using namespace std::literals;

    std::optional<DBHandler> db = ConnectToDataBase(connection_settings);
    
    if (!db.hasValue())
        return {};
    return QueryProcessor(filter, db);
}