#pragma once

#include <mutex>
#include <memory>
#include <iostream>

template <typename T>
class Singleton {
    // ʹ��protected���ι��캯����Ϊ���ڼ̳�singleton�����п��Թ������
protected:
    // =default��ʽָʾ����������Ĭ�ϳ�Ա����,c++11��������
    Singleton() = default;
    // ������������Ϳ�����ֵ
    // =delete��ʽ��ֹ����������ĳЩĬ�ϳ�Ա����
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator = (const Singleton<T>& st) = delete;
    // ����ָ��ĵ���ʵ���������Զ����գ������ֶ�����
    // ��̬��Ա������Ϊ����ʹ��ʱֻ����һ��ʵ��
    static std::shared_ptr<T> _instance;

public:
    // ��̬��Ա������������ʵ���̰߳�ȫ��ʵ�������ͷ��ʡ����ⲻ��Ҫ����Դ���ĺ��ظ���������Ŀ���
    static std::shared_ptr<T> GetInstance() {
        // ����ȷ��ĳ���ض��ĺ����������ڶ��̻߳�����ֻ��ִ��һ��
        static std::once_flag s_flag;
        // �������̰߳�ȫ�ģ����ڲ�ʹ�û������������� std::once_flag ״̬�ķ��ʡ�
        std::call_once(s_flag, [&]() {
            // std::make_shared��Ҫ�ڳ�ʼ��ʱ����֪��Ҫ����Ķ������ͣ�����ֱ����lambda���ʽ��ʹ��������ʼ��һ����̬��Ա���������Ǹñ����Ǿֲ��ģ������Υ��������ԭ��
            _instance = std::shared_ptr<T>(new T);
            });

        return _instance;
    }

    void PrintAddress() {
        // ��ȡ std::shared_ptr �������ԭʼָ�루����ָ�룩
        std::cout << _instance.get() << std::endl;
    }

    ~Singleton() {
        std::cout << "singleton destory" << std::endl;
    }
};

// ��ľ�̬��Ա���������ʼ��
template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

