#pragma once

#include <vector>

template <typename T>
class Node {
public:
	Node(const T& value) : m_Data(value) {}

	~Node() {
		for (Node* child : m_Children) {
			delete child;
		}
	}

	const std::vector<Node<T>*>& GetChildren() const {
		return m_Children;
	}

	const T& GetData() const {
		return m_Data;
	}

	Node<T>* AddChild(const T& child) {
		for(int i = 0; i < m_Children.size(); i++) {
			if(m_Children[i]->m_Data.Name == child.Name)
				return m_Children[i];
		}

		Node<T>* newNode = new Node<T>(child);
		m_Children.push_back(newNode);
		return newNode;
	}
private:
	T m_Data;
	std::vector<Node<T>*> m_Children;
};

template <typename T>
class NaryTree {
public:
	NaryTree(const T& rootValue) : m_RootNode(new Node<T>(rootValue)) {}

	~NaryTree() {
		delete m_RootNode;
	}

	Node<T>* AddChild(Node<T>* parent, const T& value) {
		return parent->AddChild(value);
	}

	Node<T>* GetRoot() const {
		return m_RootNode;
	}

private:
	Node<T>* m_RootNode;
};
