
// Dynamic method dispatch on argument types requires rapid and
// efficient mapping of argument types to methods. We achieve this by
// creating a new Tuple type whose hash function is based on the
// pointer of the argument types as opposed to the hash values of the
// argument themselves. This also avoids the need to convert arguments
// to their types and to calculate the hash values.

// Copied with minor but above crucial variation from tupleobject.c
static long
Our_Funky_tuplehash(PyTupleObject *v)
{
	register long x, y;
	register int len = v->ob_size;
	register PyObject **p;
	long mult = 1000003L;
	x = 0x345678L;
	p = v->ob_item;
	while (--len >= 0) {
	        // Pyreflex changes;
	        //y = PyObject_Hash(*p++);
	        y = (long)((*p++)->ob_type);
		if (y == -1)
			return -1;
		x = (x ^ y) * mult;
		mult += 82520L + len + len;
	}
	x += 97531L;
	if (x == -1)
		x = -2;
	return x;
}

// The hash function must have a compatible comparison

static PyObject *
Our_Funky_tuplerichcompare(PyObject *v, PyObject *w, int op)
{
	PyTupleObject *vt, *wt;
	int i;
	int vlen, wlen;

	if (!PyTuple_Check(v) || !PyTuple_Check(w)) {
		Py_INCREF(Py_NotImplemented);
		return Py_NotImplemented;
	}

	vt = (PyTupleObject *)v;
	wt = (PyTupleObject *)w;

	vlen = vt->ob_size;
	wlen = wt->ob_size;

	/* Note: the corresponding code for lists has an "early out"
	 * test here when op is EQ or NE and the lengths differ.  That
	 * pays there, but Tim was unable to find any real code where
	 * EQ/NE tuple compares don't have the same length, so testing
	 * for it here would have cost without benefit.
	 */

	/* Search for the first index where items are different.
	 * Note that because tuples are immutable, it's safe to reuse
	 * vlen and wlen across the comparison calls.
	 */

	bool items_match = true;

	for (i = 0; i < vlen && i < wlen; i++) {

         	// Pyreflex change:
	        //int k = PyObject_RichCompareBool(vt->ob_item[i],
		//				 wt->ob_item[i], Py_EQ);
                int k = (vt->ob_item[i]->ob_type == wt->ob_item[i]->ob_type);
		//if (k < 0)
		//	return NULL;
		if (!k) {
		        items_match = false;
			break;
		}
	}
	// (Pyreflex) We only care about Py_EQ in this type of tuple,
	// so we only need to check that the lengths match, and we are
	// done
	PyObject* res;
	if (items_match && vlen == wlen)
	        res = Py_True;
	else
      	        res = Py_False;
	Py_INCREF(res);
	return res;


	//if (i >= vlen || i >= wlen) {
	//	/* No more items to compare -- compare sizes */
	//	int cmp;
	//	PyObject *res;
	//	switch (op) {
	//	case Py_LT: cmp = vlen <  wlen; break;
	//	case Py_LE: cmp = vlen <= wlen; break;
	//	case Py_EQ: cmp = vlen == wlen; break;
	//	case Py_NE: cmp = vlen != wlen; break;
	//	case Py_GT: cmp = vlen >  wlen; break;
	//	case Py_GE: cmp = vlen >= wlen; break;
	//	default: return NULL; /* cannot happen */
	//	}
	//	if (cmp)
	//		res = Py_True;
	//	else
	//		res = Py_False;
	//	Py_INCREF(res);
	//	return res;
	//}
	//
	///* We have an item that differs -- shortcuts for EQ/NE */
	//if (op == Py_EQ) {
	//	Py_INCREF(Py_False);
	//	return Py_False;
	//}
	//if (op == Py_NE) {
	//	Py_INCREF(Py_True);
	//	return Py_True;
	//}
	//
	///* Compare the final item again using the proper operator */
	//return PyObject_RichCompare(vt->ob_item[i], wt->ob_item[i], op);
}

// #########################

PyDoc_STRVAR(Funky_tuple_doc,
"Same as the builtin tuple ...\n"
"except that __hash__ and __eq__ consider the types\n"
"of the items rather than the items themselves.\n"
"To be used only in method dispatching.");

// (Pyreflex)
// We cannot create this type dynamically by calling type("funky",
// (tuple,), {}), because that makes a heap type, and it is impossible
// to rebind __class__ (or rather ob_type) if a heap type is involved.

PyTypeObject FunkyTuple_Type = {
        PyObject_HEAD_INIT(&PyType_Type)
	0,
	"Funkytuple",
	sizeof(PyTupleObject) - sizeof(PyObject *),
	sizeof(PyObject *),
	0,//(destructor)tupledealloc,		/* tp_dealloc */
	0,//(printfunc)tupleprint,		/* tp_print */
	0,					/* tp_getattr */
	0,					/* tp_setattr */
	0,					/* tp_compare */
	0,//(reprfunc)tuplerepr,		/* tp_repr */
	0,					/* tp_as_number */
	0,//&tuple_as_sequence,			/* tp_as_sequence */
	0,//&tuple_as_mapping,			/* tp_as_mapping */
	(hashfunc)Our_Funky_tuplehash,		/* tp_hash */
	0,					/* tp_call */
	0,					/* tp_str */
	0,//PyObject_GenericGetAttr,		/* tp_getattro */
	0,					/* tp_setattro */
	0,					/* tp_as_buffer */
	// tp_traverse, tp_clear, Py_TPFLAGS_HAVE_GC and
	// Py_TPFLAGS_HAVE_RICHCOMPARE are involved in a complicated
	// inheritance rule, related to GC support. Read section 10.3
	// in the C API Reference Manual.
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_RICHCOMPARE |
		Py_TPFLAGS_BASETYPE,		/* tp_flags */
	Funky_tuple_doc,			/* tp_doc */
 	0,//(traverseproc)tupletraverse,	/* tp_traverse */
	0,					/* tp_clear */
	Our_Funky_tuplerichcompare,		/* tp_richcompare */
	0,					/* tp_weaklistoffset */
	0,//tuple_iter,	    			/* tp_iter */
	0,					/* tp_iternext */
	0,///tuple_methods,			/* tp_methods */
	0,					/* tp_members */
	0,					/* tp_getset */
	&PyTuple_Type,				/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	0,					/* tp_dictoffset */
	0,					/* tp_init */
	0,					/* tp_alloc */
	0,//tuple_new,				/* tp_new */
	0,//PyObject_GC_Del,        		/* tp_free */
};

// Used to switch the type of tuples of arguments in order to speed up method dispatch.

PyObject* funkize(PyObject*, PyObject* tuple) {
  tuple->ob_type = &FunkyTuple_Type;
  Py_INCREF(Py_None);
  return Py_None;
} 

