#pragma once

#include "BaseItem.h"
#include <QPainter>
#include <QRect>
#include <QString>
#include <memory>
#include <vector>

// картотека: хранит сотрудников через указатель на базовый класс
class Container {
public:
    bool saveToFile(const QString& filename) const;
    bool loadFromFile(const QString& filename);
    void clear() { m_items.clear(); }
    size_t size() const { return m_items.size(); }
    void addItem(std::shared_ptr<BaseItem> item) { if (item) m_items.push_back(std::move(item)); }
    void removeItem(int index);
    void replaceItem(int index, std::shared_ptr<BaseItem> item);
    std::shared_ptr<BaseItem> getItem(int index) const;

    template<class F>
    void forEach(F&& f) const {
        for (const auto& item : m_items) f(item);
    }

    void paintTable(QPainter* painter, const QRect& viewRect) const;

private:
    std::vector<std::shared_ptr<BaseItem>> m_items;
};
