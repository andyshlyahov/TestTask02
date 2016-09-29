
template<typename key_type, typename value_type>
class Container
{
private:
	RBTree<key_type, value_type> *tree;

public:
	Container()
	{
		tree = new RBTree<key_type, value_type>;
	}
	void add(key_type key, value_type value)
	{
		tree->add(key, value);
	}
	void remove(key_type key, value_type value)
	{
		tree->remove(key, value);
	}
	std::queue<value_type> get(key_type key)
	{
		return tree->get(key);
	}
	void destroy()
	{
		tree->~RBTree();
	}
};
