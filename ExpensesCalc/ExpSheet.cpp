// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "ExpSheet.h"

bool ExpSheet::Add(std::string_view label, double val)
{
    Entry e;
    e.label = label;
    e.value = val;

    bool canInsert = std::find(m_entries.begin(), m_entries.end(), e) == m_entries.end();
    
    if (canInsert)
        m_entries.emplace_back(std::move(e));

    return canInsert;
}

bool ExpSheet::Del(std::string_view label)
{
    Entry e;
    e.label = label;
    auto el = std::find(m_entries.begin(), m_entries.end(), e);

    if (el != m_entries.end())
    {
        m_entries.erase(el);
        return true;
    }

    return false;
}

void ExpSheet::List(std::ostream& os) const
{
    os << std::fixed << std::setprecision(2);

    size_t index = 0;
    for (const Entry& e : m_entries)
    {
        os << "(" << index << ") " << e.label << " -> " << e.value << std::endl;
    }
}

double ExpSheet::Eval() const
{
    double total = 0.0;

    for (const Entry& e : m_entries)
        total += e.value;

    return total;
}
