#include "LmdbProduccionRepository.h"
#include <nlohmann/json.hpp>
#include <lmdb.h>
#include <stdexcept>

using json = nlohmann::json;

namespace Infrastructure {

LmdbProduccionRepository::LmdbProduccionRepository(std::shared_ptr<LmdbDatabase> db)
    : m_db(std::move(db)) {}

// ─── JSON serialization ─────────────────────────────────────────────────────

std::string LmdbProduccionRepository::toJson(const Domain::Produccion& p) {
    json j;
    j["id"]     = p.id;
    j["prenez"] = p.prenez;
    j["ordeno"] = p.ordeno;

    j["partos"] = json::array();
    for (const auto& parto : p.partos)
        j["partos"].push_back({{"id", parto.id}, {"fecha", parto.fecha}});

    j["registro_leche"] = json::array();
    for (const auto& r : p.registroLeche)
        j["registro_leche"].push_back({{"fecha", r.fecha}, {"valor", r.valor}});

    j["registro_carne"] = json::array();
    for (const auto& r : p.registroCarne)
        j["registro_carne"].push_back({{"fecha", r.fecha}, {"valor", r.valor}});

    return j.dump();
}

Domain::Produccion LmdbProduccionRepository::fromJson(const std::string& jsonStr) {
    json j = json::parse(jsonStr);
    Domain::Produccion p;

    p.id     = j.at("id").get<std::string>();
    p.prenez = j.at("prenez").get<bool>();
    p.ordeno = j.at("ordeno").get<bool>();

    for (const auto& item : j.at("partos"))
        p.partos.push_back({item.at("id").get<std::string>(),
                            item.at("fecha").get<std::string>()});

    for (const auto& item : j.at("registro_leche"))
        p.registroLeche.push_back({item.at("fecha").get<std::string>(),
                                   item.at("valor").get<double>()});

    for (const auto& item : j.at("registro_carne"))
        p.registroCarne.push_back({item.at("fecha").get<std::string>(),
                                   item.at("valor").get<double>()});
    return p;
}

// ─── load / save (primitivos LMDB) ──────────────────────────────────────────

std::optional<Domain::Produccion>
LmdbProduccionRepository::load(const std::string& id) const {
    MDB_txn* txn = nullptr;
    if (mdb_txn_begin(m_db->env(), nullptr, MDB_RDONLY, &txn) != 0)
        return std::nullopt;

    MDB_val key{ id.size(), const_cast<char*>(id.data()) };
    MDB_val data{};

    std::optional<Domain::Produccion> result;
    if (mdb_get(txn, m_db->dbi(), &key, &data) == 0) {
        std::string jsonStr(static_cast<char*>(data.mv_data), data.mv_size);
        result = fromJson(jsonStr);
    }

    mdb_txn_abort(txn);
    return result;
}

bool LmdbProduccionRepository::save(const Domain::Produccion& p) {
    std::string jsonStr = toJson(p);

    MDB_txn* txn = nullptr;
    if (mdb_txn_begin(m_db->env(), nullptr, 0, &txn) != 0)
        return false;

    MDB_val key{ p.id.size(), const_cast<char*>(p.id.data()) };
    MDB_val data{ jsonStr.size(), jsonStr.data() };

    bool ok = mdb_put(txn, m_db->dbi(), &key, &data, 0) == 0;
    if (ok) mdb_txn_commit(txn);
    else    mdb_txn_abort(txn);
    return ok;
}

// ─── CRUD principal ──────────────────────────────────────────────────────────

std::optional<Domain::Produccion>
LmdbProduccionRepository::getById(const std::string& id) const {
    return load(id);
}

bool LmdbProduccionRepository::insert(const Domain::Produccion& p) {
    return save(p);
}

bool LmdbProduccionRepository::deleteById(const std::string& id) {
    MDB_txn* txn = nullptr;
    if (mdb_txn_begin(m_db->env(), nullptr, 0, &txn) != 0)
        return false;

    MDB_val key{ id.size(), const_cast<char*>(id.data()) };
    bool ok = mdb_del(txn, m_db->dbi(), &key, nullptr) == 0;
    if (ok) mdb_txn_commit(txn);
    else    mdb_txn_abort(txn);
    return ok;
}

// ─── Campos simples ──────────────────────────────────────────────────────────

bool LmdbProduccionRepository::updatePrenez(const std::string& id, bool prenez) {
    auto p = load(id);
    if (!p) return false;
    p->prenez = prenez;
    return save(*p);
}

bool LmdbProduccionRepository::updateOrdeno(const std::string& id, bool ordeno) {
    auto p = load(id);
    if (!p) return false;
    p->ordeno = ordeno;
    return save(*p);
}

// ─── Partos ──────────────────────────────────────────────────────────────────

bool LmdbProduccionRepository::addParto(const std::string& id, const Domain::Parto& parto) {
    auto p = load(id);
    if (!p) return false;
    p->partos.push_back(parto);
    return save(*p);
}

bool LmdbProduccionRepository::updateParto(const std::string& id, const Domain::Parto& parto) {
    auto p = load(id);
    if (!p) return false;
    for (auto& existing : p->partos) {
        if (existing.id == parto.id) {
            existing = parto;
            return save(*p);
        }
    }
    return false;
}

bool LmdbProduccionRepository::deleteParto(const std::string& id, const std::string& partoId) {
    auto p = load(id);
    if (!p) return false;
    auto& v = p->partos;
    auto it = std::ranges::find_if(v, [&](const Domain::Parto& x){ return x.id == partoId; });
    if (it == v.end()) return false;
    v.erase(it);
    return save(*p);
}

// ─── Registro leche ──────────────────────────────────────────────────────────

bool LmdbProduccionRepository::addRegistroLeche(const std::string& id,
                                                 const Domain::RegistroFecha& r) {
    auto p = load(id);
    if (!p) return false;
    p->registroLeche.push_back(r);
    return save(*p);
}

bool LmdbProduccionRepository::updateRegistroLeche(const std::string& id,
                                                    const std::string& fecha,
                                                    double nuevoValor) {
    auto p = load(id);
    if (!p) return false;
    for (auto& r : p->registroLeche) {
        if (r.fecha == fecha) {
            r.valor = nuevoValor;
            return save(*p);
        }
    }
    return false;
}

bool LmdbProduccionRepository::deleteRegistroLeche(const std::string& id,
                                                    const std::string& fecha) {
    auto p = load(id);
    if (!p) return false;
    auto& v = p->registroLeche;
    auto it = std::ranges::find_if(v, [&](const Domain::RegistroFecha& r){ return r.fecha == fecha; });
    if (it == v.end()) return false;
    v.erase(it);
    return save(*p);
}

// ─── Registro carne ──────────────────────────────────────────────────────────

bool LmdbProduccionRepository::addRegistroCarne(const std::string& id,
                                                 const Domain::RegistroFecha& r) {
    auto p = load(id);
    if (!p) return false;
    p->registroCarne.push_back(r);
    return save(*p);
}

bool LmdbProduccionRepository::updateRegistroCarne(const std::string& id,
                                                    const std::string& fecha,
                                                    double nuevoValor) {
    auto p = load(id);
    if (!p) return false;
    for (auto& r : p->registroCarne) {
        if (r.fecha == fecha) {
            r.valor = nuevoValor;
            return save(*p);
        }
    }
    return false;
}

bool LmdbProduccionRepository::deleteRegistroCarne(const std::string& id,
                                                    const std::string& fecha) {
    auto p = load(id);
    if (!p) return false;
    auto& v = p->registroCarne;
    auto it = std::ranges::find_if(v, [&](const Domain::RegistroFecha& r){ return r.fecha == fecha; });
    if (it == v.end()) return false;
    v.erase(it);
    return save(*p);
}

} // namespace Infrastructure