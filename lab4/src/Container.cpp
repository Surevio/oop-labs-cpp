#include "Container.h"
#include "DerivedItem.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

// оба типа регистрируются в одинаковом порядке при сохранении и загрузке,
// иначе DerivedItem нельзя сохранить через shared_ptr<BaseItem>
template<class Archive>
static void registerTypes(Archive& ar) {
    ar.template register_type<BaseItem>();
    ar.template register_type<DerivedItem>();
}

bool Container::saveToFile(const QString& filename) const {
    try {
        std::ofstream file(std::filesystem::path(filename.toStdWString()));
        if (!file) return false;

        boost::archive::text_oarchive archive(file);
        registerTypes(archive);
        archive << m_items;
        return true;
    }
    catch (const std::exception&) {
        return false;
    }
}

bool Container::loadFromFile(const QString& filename) {
    try {
        std::ifstream file(std::filesystem::path(filename.toStdWString()));
        if (!file) return false;

        std::vector<std::shared_ptr<BaseItem>> loaded;
        boost::archive::text_iarchive archive(file);
        registerTypes(archive);
        archive >> loaded;
        m_items.swap(loaded);
        return true;
    }
    catch (const std::exception&) {
        return false;
    }
}

void Container::removeItem(int index) {
    if (index < 0 || index >= static_cast<int>(m_items.size())) return;
    m_items.erase(m_items.begin() + index);
}

void Container::replaceItem(int index, std::shared_ptr<BaseItem> item) {
    if (index < 0 || index >= static_cast<int>(m_items.size()) || !item) return;
    m_items[index] = std::move(item);
}

std::shared_ptr<BaseItem> Container::getItem(int index) const {
    if (index < 0 || index >= static_cast<int>(m_items.size())) return nullptr;
    return m_items[index];
}

void Container::paintTable(QPainter* painter, const QRect& viewRect) const {
    if (!painter || m_items.empty()) return;

    int currentY = viewRect.top() + 30;

    painter->save();
    painter->setFont(QFont("Arial", 12, QFont::Bold));
    painter->drawText(viewRect.left() + 10, currentY, QStringLiteral("Картотека сотрудников:"));
    painter->restore();
    currentY += 30;

    // обход контейнера через for_each + bind, paint сам сдвигает currentY
    std::for_each(m_items.begin(), m_items.end(),
        std::bind(&BaseItem::paint, std::placeholders::_1, painter, std::ref(currentY)));
}
