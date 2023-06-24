// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "ExpSheet.h"

void ExpSheet::Entry::Serialize(std::ostream& out) const
{
    out.write(label.c_str(), label.length() + 1);
    out.write((char*)&value, sizeof(double));
}

void ExpSheet::Entry::Deserialize(std::istream& in)
{
    std::stringstream ss;
    char c;

    do
    {
        in.read(&c, 1);
        if (c != '\0')
            ss << c;

    } while (c != '\0');

    label = ss.str();
    in.read((char*)&value, sizeof(double));
}

bool ExpSheet::Open(const std::filesystem::path& dataFile)
{
    auto path = dataFile;
    path.replace_extension(".pexc");

    std::ifstream fileIn(path, std::ios::in | std::ios::binary);

    if (fileIn.is_open())
    {
        m_path = path;

        size_t numElements = 0;
        fileIn.read((char*)&numElements, sizeof(size_t));
        m_entries.clear();
        
        for (size_t i = 0; i < numElements; i++)
        {
            Entry e;
            e.Deserialize(fileIn);
            m_entries.emplace_back(std::move(e));
        }

        return true;
    }
    return false;
}

bool ExpSheet::Save(const std::filesystem::path& dataFile) const
{
    auto path = dataFile;
 
    if (dataFile.empty())
        if (m_path.empty())
            return false;
        else
            path = m_path;

    path.replace_extension(".pexc");

    std::ofstream fileOut(path, std::ios::out | std::ios::trunc | std::ios::binary);

    if (fileOut.is_open())
    {
        size_t numElements = m_entries.size(); // Dumb way of doing this. Research better way
        fileOut.write((char*)&numElements, sizeof(size_t));

        for (const Entry& e : m_entries)
        {
            e.Serialize(fileOut);
        }
        return true;
    }
    return false;
}

void ExpSheet::Clear()
{
    m_path = "";
    m_entries.clear();
}

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
