#ifndef FUNCTIONFACTORY_H
#define FUNCTIONFACTORY_H

#include <QString>
#include <QMap>
#include <QList>
#include <QDebug>
#include <vector>
using std::vector;

class FunctionFactory
{
public:
    FunctionFactory()
    {
    }
    virtual ~FunctionFactory()
    {
    }

    virtual double function(size_t xi, size_t N) = 0;

    virtual vector<double> data(size_t N)
    {
        // create empty vector
        vector<double> v = vector<double>(N);
        // fill with function
        for(std::vector<int>::size_type xi = 0; xi != N; xi++) {
            v[xi] = function(xi, N);
        }
        return v;
    }

    virtual QString description() = 0;
    virtual void parameterChanged() {}

    virtual void setParameter(const QString name, double value){

        if ( ! mapValue.isEmpty() ) {
            if (mapValue.contains(name)) {
                mapValue[name] = value;
                parameterChanged();
                return;
            }
        }
        // value either empty or name not in map
        mapValue.insert(name, value);
        parameterChanged();
    }

    virtual double parameter(const QString name)
    {
        double value = 0;
        if ( ! mapValue.isEmpty() ) {
            if (mapValue.contains(name)) {
                value = mapValue[name];
            } else {
                qDebug() << "Error: unkown parameter: " << name;
                // report error, type not in map
            }
        }
        return value;
    }

    //! set fixed minimum, indipendent of N
    //!  for min values depended on <name> overload this function
    virtual void setMin(const QString name, double value)
    {
        mapValueMin[name] = value;
    }

    //! set fixed maximum, indipendent of N
    //!  for max values depended on <name> overload this function
    virtual void setMax(const QString name, double value)
    {
        mapValueMax[name] = value;
    }

    virtual double min(const QString name, size_t N)
    {
        Q_UNUSED(N);
        // returns valid value, otherwise inserts 0 for key!
        return mapValueMin[name];
    }

    virtual double max(const QString name, size_t N)
    {
        Q_UNUSED(N);
        // returns valid value, otherwise inserts 0 for key!
        return mapValueMax[name];
    }

    const QList<QString> parameterList() const
    {
        return mapValue.uniqueKeys();
    }

private:
    QMap<QString, double > mapValue;
    QMap<QString, double > mapValueMin;
    QMap<QString, double > mapValueMax;
};


class OneFunction : public FunctionFactory
{
public:
    OneFunction() : FunctionFactory()
    {}

    double function(size_t xi, size_t N)
    {
        Q_UNUSED(xi);
        Q_UNUSED(N);
        return 1.0;
    }

    vector<double> function(size_t N)
    {
        return vector<double>(N, 1);
    }

    QString description()
    {
        return QString("One (constant)");
    }

};


class NullFunction : public FunctionFactory
{
public:
    NullFunction() : FunctionFactory()
    {}

    double function(size_t xi, size_t N)
    {
        Q_UNUSED(xi);
        Q_UNUSED(N);
        return 0.0;
    }

    vector<double> function(size_t N)
    {
        return vector<double>(N, 0);
    }

    QString description()
    {
        return QString("Null (constant)");
    }

};


#endif // FUNCTIONFACTORY_H
