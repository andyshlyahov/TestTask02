#include <queue>

#define BLACK false
#define RED   true

template<typename key_type, typename value_type>
class RBTree
{

private:
	class  RBTreeNode
	{
	public:
		key_type   key;
		value_type value;
		RBTreeNode *left;
		RBTreeNode *right;
		bool color;
		RBTreeNode(key_type key, value_type value)
		{
			this->key = key;
			this->value = value;
			this->color = RED;
		}
	};
	RBTreeNode *root;

	RBTreeNode *insert(RBTreeNode *p, key_type key, value_type value);
	std::queue<value_type> getValues(RBTreeNode *p, std::queue<value_type> &values);
	std::queue<value_type> get(RBTreeNode *p, key_type key);

	RBTreeNode *rotateLeft(RBTreeNode *p);
	RBTreeNode *rotateRight(RBTreeNode *p);
	RBTreeNode *moveRedLeft(RBTreeNode *p);
	RBTreeNode *moveRedRight(RBTreeNode *p);

	RBTreeNode *deleteMin(RBTreeNode *p);
	RBTreeNode *fixUp(RBTreeNode *p);
	RBTreeNode *remove(RBTreeNode *p, key_type key, value_type value);
	RBTreeNode *successorNode(RBTreeNode *p);
	void destroy(RBTreeNode *root);

	bool isRed(RBTreeNode *p);
	void colorFlip(RBTreeNode *p);
	void deleteMin();

public:
	explicit RBTree() {  }
	~RBTree() { destroy(root); root = nullptr; }

	void add(key_type key, value_type value);
	void remove(key_type key, value_type value);
	std::queue<value_type> get(key_type key);
};

template<typename key_type, typename value_type>
void RBTree<key_type, value_type>::add(key_type key, value_type value)
{
	root = insert(root, key, value);
	root->color = BLACK;
}

template<typename key_type, typename value_type>
void RBTree<key_type, value_type>::remove(key_type key, value_type value)
{
	if (root == nullptr)
	{
		return;
	}
	root = remove(root, key, value);

	if (root != nullptr)
	{
		root->color = BLACK;
	}
}

template<typename key_type, typename value_type>
std::queue<value_type> RBTree<key_type, value_type>::get(key_type key)
{
	return get(root, key);
}



template<typename key_type, typename value_type>
typename RBTree<key_type, value_type>::RBTreeNode *RBTree<key_type, value_type>::insert(RBTreeNode *p, key_type key, value_type value)
{
	if (p == nullptr)
	{
		return new RBTreeNode(key, value);
	}

	if (isRed(p->left) && isRed(p->right))
	{
		colorFlip(p);
	}

	if (key < p->key || (key == p->key && value < p->value))
	{
		p->left = insert(p->left, key, value);
	}
	else if (key > p->key || (key == p->key && value > p->value))
	{
		p->right = insert(p->right, key, value);
	}
	else
	{
		return p;
	}

	if (isRed(p->right))
	{
		p = rotateLeft(p);
	}

	if (isRed(p->left) && isRed(p->left->left))
	{
		p = rotateRight(p);
	}

	return p;
}

template<typename key_type, typename value_type>
std::queue<value_type> RBTree<key_type, value_type>::getValues(RBTreeNode *p, std::queue<value_type> &values)
{
	values.push(p->value);

	if (p->left != nullptr)
	{
		if (p->key == p->left->key)
		{
			getValues(p->left, values);
		}
		else
		{
			RBTreeNode *tmp = p->left;

			while (tmp->right != nullptr)
			{
				tmp = tmp->right;
				if (tmp->key == p->key)
				{
					getValues(tmp, values);
					break;
				}
			}
		}
	}

	if (p->right != nullptr)
	{
		if (p->key == p->right->key)
		{
			getValues(p->right, values);
		}
		else
		{
			RBTreeNode *tmp = p->right;

			while (tmp->left != nullptr)
			{
				tmp = tmp->left;
				if (tmp->key == p->key)
				{
					getValues(tmp, values);
					break;
				}
			}
		}
	}

	return values;
}

template<typename key_type, typename value_type>
std::queue<value_type> RBTree<key_type, value_type>::get(RBTreeNode *p, key_type key)
{
	while (p != nullptr)
	{
		if (key < p->key)
		{
			p = p->left;
		}
		else if (key > p->key)
		{
			p = p->right;
		}
		else
		{
			std::queue<value_type> values;
			return getValues(p, values);
		}
	}

	return std::queue<value_type>();
}

template<typename key_type, typename value_type>
typename RBTree<key_type, value_type>::RBTreeNode *RBTree<key_type, value_type>::rotateLeft(RBTreeNode *p)
{
	RBTreeNode  *x = p->right;
	p->right = x->left;
	x->left = p;
	x->color = x->left->color;
	x->left->color = RED;
	return x;
}

template<typename key_type, typename value_type>
typename RBTree<key_type, value_type>::RBTreeNode * RBTree<key_type, value_type>::rotateRight(RBTreeNode *p)
{
	RBTreeNode *x = p->left;
	p->left = x->right;
	x->right = p;
	x->color = x->right->color;
	x->right->color = RED;
	return x;
}

template<typename key_type, typename value_type>
typename RBTree<key_type, value_type>::RBTreeNode * RBTree<key_type, value_type>::moveRedLeft(RBTreeNode *p)
{
	colorFlip(p);

	if (isRed(p->right->left))
	{
		p->right = rotateRight(p->right);
		p = rotateLeft(p);
		colorFlip(p);
	}
	return p;
}

template<typename key_type, typename value_type>
typename RBTree<key_type, value_type>::RBTreeNode * RBTree<key_type, value_type>::moveRedRight(RBTreeNode *p)
{
	colorFlip(p);

	if (isRed(p->left->left))
	{
		p = rotateRight(p);
		colorFlip(p);
	}
	return p;
}

template<typename key_type, typename value_type>
typename RBTree<key_type, value_type>::RBTreeNode *RBTree<key_type, value_type>::deleteMin(RBTreeNode *p)
{
	if (p->left == nullptr)
	{
		delete p;
		p = nullptr;
		return p;
	}

	if (!isRed(p->left) && !isRed(p->left->left))
	{
		p = moveRedLeft(p);
	}

	p->left = deleteMin(p->left);

	return fixUp(p);
}

template<typename key_type, typename value_type>
typename RBTree<key_type, value_type>::RBTreeNode *RBTree<key_type, value_type>::fixUp(RBTreeNode *p)
{
	if (isRed(p->right))
	{
		p = rotateLeft(p);
	}

	if (isRed(p->left) && isRed(p->left->left))
	{
		p = rotateRight(p);
	}

	if (isRed(p->left) && isRed(p->right))
	{
		colorFlip(p);
	}

	return p;
}

template<typename key_type, typename value_type>
typename RBTree<key_type, value_type>::RBTreeNode *RBTree<key_type, value_type>::remove(RBTreeNode *p, key_type key, value_type value)
{
	if (key < p->key || (key == p->key && value < p->value))
	{
		if (!isRed(p->left) && !isRed(p->left->left))
		{
			p = moveRedLeft(p);
		}
		p->left = remove(p->left, key, value);

	}
	else
	{
		if (isRed(p->left))
		{
			p = rotateRight(p);
		}

		if ((key == p->key && value == p->value) && (p->right == nullptr))
		{
			delete p;
			p = nullptr;
			return p;
		}

		if (!isRed(p->right) && !isRed(p->right->left))
		{
			p = moveRedRight(p);
		}

		if (key == p->key && value == p->value)
		{
			RBTreeNode *successor = successorNode(p);
			p->value = successor->value;
			p->key = successor->key;
			p->right = deleteMin(p->right);
		}
		else
		{
			p->right = remove(p->right, key, value);
		}
	}

	return fixUp(p);
}

template<typename key_type, typename value_type> inline
typename RBTree<key_type, value_type>::RBTreeNode *RBTree<key_type, value_type>::successorNode(RBTreeNode *p)
{
	p = p->right;

	while (p->left != nullptr)
	{
		p = p->left;
	}

	return p;
}

template<typename key_type, typename value_type>
void RBTree<key_type, value_type>::destroy(RBTreeNode *current)
{
	if (current == nullptr)
	{
		return;
	}
	destroy(current->left);
	destroy(current->right);

	delete current;
	current = nullptr;
}

template<typename key_type, typename value_type>
bool RBTree<key_type, value_type>::isRed(RBTreeNode *p)
{
	return (p == nullptr) ? false : (p->color == RED);
}

template<typename key_type, typename value_type>
void RBTree<key_type, value_type>::colorFlip(RBTreeNode *p)
{
	p->color = !p->color;
	p->left->color = !p->left->color;
	p->right->color = !p->right->color;
}

template<typename key_type, typename value_type>
void RBTree<key_type, value_type>::deleteMin()
{
	root = deleteMin(root);
	root->color = BLACK;
}
