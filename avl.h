struct nil
{
    static const int balance_factor = 0;
    static const int height = 0;
};

template<int a, int b, bool c = (a > b)>
struct max
{
    static const int result = a;
};

template<int a, int b>
struct max<a, b, false>
{
    static const int result = b;
};

template<int Value, typename Left, typename Right>
struct node
{
    static const int value = Value;
    typedef Left left;
    typedef Right right;
    static const int height = max<Left::height, Right::height>::result + 1;
    static const int balance_factor = Left::height - Right::height;
};

 //find
template <typename tree, bool right, bool equals, int value>
struct search;

template <typename tree, int value>
struct find
{
    static const bool result = search<tree, (value > tree::value), (value == tree::value), value>::result;
};

template <typename tree, bool right, int value>
struct search<tree, right, true, value>
{
    static const bool result = true;
};

template <typename tree, int value>
struct search<tree, true, false, value>
{
    static const bool result = find<typename tree::right, value>::result;
};

template <typename tree, int value>
struct search<tree, false, false, value>
{
    static const bool result = find<typename tree::left, value>::result;
};

template <int value>
struct find<nil, value>
{
    static const bool result = false;
};

// rotates

template<typename node>
struct balance_factor
{
    static const int result = node::balance_factor;
};

template<typename tree, bool left>
struct get_big_rotate_type;

template<typename tree>
struct get_big_rotate_type <tree, true>
{
    static const bool big = balance_factor<typename tree::right>::result > 0;
};


template<typename tree>
struct get_big_rotate_type <tree, false>
{
    static const bool big = balance_factor<typename tree::left>::result < 0;
};

template<>
struct get_big_rotate_type <nil, false>
{
    static const bool big = false;
};

template<typename tree>
struct get_rotate_type
{
    static const bool has_right_bias = balance_factor<tree>::result == -2;
    static const bool need_rotate = has_right_bias || (balance_factor<tree>::result == 2);
    static const bool left_rotate = has_right_bias;
    static const bool big_rotate = get_big_rotate_type<tree, left_rotate>::big;
};

// balance

template <typename tree>
struct rotate_right
{
    typedef node <tree::left::value, typename tree::left::left,
    node<tree::value, typename tree::left::right, typename tree::right>> result;
};

template <typename tree>
struct rotate_left
{
    typedef node <tree::right::value,
        node<tree::value, typename tree::left, typename tree::right::left>,
        typename tree::right::right> result;
};

template <typename tree, bool needs_balance, bool rotate_left, bool big_rotate>
struct balance_case
{
    typedef get_rotate_type<tree> rotate_type;
    typedef typename balance_case<tree, rotate_type::need_rotate, rotate_type::left_rotate,
        rotate_type::big_rotate>::result result;
};

//no rotate needed
template <typename tree, bool rotate_left, bool big_rotate>
struct balance_case<tree, false, rotate_left, big_rotate>
{
    typedef tree result;
};

//left small rotate
template <typename tree>
struct balance_case<tree, true, true, false>
{
    typedef typename rotate_left<tree>::result result;
};

//right small rotate
template <typename tree>
struct balance_case<tree, true, false, false>
{
    typedef typename rotate_right<tree>::result result;
};

//left big rotate
template <typename tree>
struct balance_case<tree, true, true, true>
{
    typedef typename rotate_left<node<tree::value, typename tree::left,
            typename rotate_right<typename tree::right>::result>>::result result;
};

//right big rotate
template <typename tree>
struct balance_case<tree, true, false, true>
{
    typedef typename rotate_right<node<tree::value,
        typename rotate_left<typename tree::left>::result,
        typename tree::right>>::result result;
};

template <typename tree>
struct balance
{
    typedef get_rotate_type<tree> rotate_type;
    typedef typename balance_case<tree, rotate_type::need_rotate, rotate_type::left_rotate,
        rotate_type::big_rotate>::result result;
};

//use add to insert, insert - helping function
template <typename tree, bool right, int value>
struct insert;

template <typename tree, int value>
struct add
{
    typedef typename balance<typename insert<tree, (value > tree::value), value>::result>::result result;
};

template<int value>
struct add<nil, value>
{
    typedef node<value, nil, nil> result;
};

template <typename tree, int value>
struct insert<tree, true, value>
{
    typedef node<tree::value, typename tree::left,
    typename add<typename tree::right, value>::result> result;
};

template <typename tree, int value>
struct insert<tree, false, value>
{
    typedef node<tree::value, typename add<typename tree::left, value>::result, typename tree::right> result;
};

//delete ------ use avl_delete to delete, avl_remove and etc are helping functions
template <typename tree, bool equal, bool right, int value>
struct avl_remove;

template <typename tree, int value>
struct avl_delete
{
    typedef typename balance<typename avl_remove<tree, (value == tree::value),
            (value > tree::value), value>::result>::result result;
};

template <int value>
struct avl_delete<nil, value>
{
    typedef nil result;
};

template <typename tree, int value>
struct avl_remove<tree, false, true, value>
{
    typedef node<tree::value, typename tree::left,
        typename avl_delete<typename tree::right, value>::result> result;
};

template <typename tree, int value>
struct avl_remove<tree, false, false, value>
{
    typedef node<tree::value, typename avl_delete<typename tree::left, value>::result, typename tree::right> result;
};


template <typename left, typename right, int value>
struct avl_delete_min
{
    typedef avl_delete_min<typename left::left, typename left::right, left::value> avl_deleted;
    typedef node<value, typename avl_deleted::result, right> result;
    static const int min = avl_deleted::min;
};

template <typename right, int value>
struct avl_delete_min<nil, right, value>
{
    typedef right result;
    static const int min = value;
};


template <typename left, typename right, int value>
struct avl_delete_node
{
    typedef avl_delete_min < typename right::left, typename right::right, right::value >  avl_deleted_min;
    typedef node < avl_deleted_min::min, left, typename avl_deleted_min::result > result;
};

template <typename right, int value>
struct avl_delete_node<nil, right, value>
{
    typedef right result;
};

template <typename left, int value>
struct avl_delete_node<left, nil, value>
{
    typedef left result;
};

template <int value>
struct avl_delete_node<nil, nil, value>
{
    typedef nil result;
};

template <typename tree, bool right, int value>
struct avl_remove<tree, true, right, value>
{
    typedef typename avl_delete_node<typename tree::left, typename tree::right, tree::value>::result result;
};
