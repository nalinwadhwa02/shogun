/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Soumyajit De, Yuyu Zhang
 */

#ifndef SUBSEQUENCE_STRING_KERNEL_H_
#define SUBSEQUENCE_STRING_KERNEL_H_

#include <shogun/lib/config.h>

#include <shogun/lib/common.h>
#include <shogun/kernel/string/StringKernel.h>

namespace shogun
{
/**
 * @brief class SubsequenceStringKernel that implements String Subsequence Kernel
 * (SSK) discussed by Lodhi et. al.[1]. A subsequence is any ordered sequence
 * of \f$n\f$ characters occurring in the text, though not necessarily contiguous.
 * More formally, string \f$u\f$ is a subsequence of string \f$s\f$, iff there
 * exists indices \f$\mathbf{i}=(i_{1},\dots,i_{|u|})\f$, with
 * \f$1\le i_{1} \le \cdots \le i_{|u|} \le |s|\f$, such that
 * \f$u_{j}=s_{i_{j}}\f$ for \f$j=1,\dots,|u|\f$, written as \f$u=s[\mathbf{i}]\f$.
 * The feature mapping \f$\phi\f$ in this scenario is given by
 * \f[
 *    \phi_{u}(s)=\sum_{\mathbf{i}:u=s[\mathbf{i}]}\lambda^{l(\mathbf{i})}
 * \f]
 * for some \f$lambda\le 1\f$, where \f$l(\mathbf{i})\f$ is the length of the
 * subsequence in \f$s\f$, given by \f$i_{|u|}-i_{1}+1\f$.
 * The kernel here is an inner product in the feature space generated by all
 * subsequences of length \f$n\f$.
 * \f[
 * 	K_{n}(s,t)=\sum_{u\in\Sigma^{n}}\langle \phi_{u}(s), \phi_{u}(t)\rangle
 * 	= \sum_{u\in\Sigma^{n}}\sum_{\mathbf{i}:u=s[\mathbf{i}]}
 *	\sum_{\mathbf{j}:u=t[\mathbf{j}]}\lambda^{l(\mathbf{i})+l(\mathbf{j})}
 * \f]
 * Since the subsequences are weighted by the exponentially decaying factor
 * \f$\lambda\f$ of their full length in the text, more weight is given to those
 * occurrences that are nearly contiguous. A direct computation is infeasible
 * since the dimension of the feature space grows exponentially with \f$n\f$.
 * The paper describes an efficient computation approach using a dynamic
 * programming technique.
 *
 * The implementation is inspired from Razvan C. Bunescu's
 * (http://ace.cs.ohiou.edu/~razvan/) SSK core library.
 *
 * Reference:
 * [1] Text Classification using String Kernels, Lodhi et. al. Journal of Machine
 * Learning Research 2(2002), 419-444.
 */
class SubsequenceStringKernel: public StringKernel<char>
{
public:
	/** default constructor  */
	SubsequenceStringKernel();

	/**
	 * constructor
	 *
	 * @param size cache size
	 * @param maxlen maximum length of the subsequence
	 * @param lambda the penalty parameter
	 */
	SubsequenceStringKernel(int32_t size, int32_t maxlen, float64_t lambda);

	/**
	 * constructor
	 *
	 * @param lhs features of left-hand side
	 * @param rhs features of right-hand side
	 * @param maxlen maximum length of the subsequence
	 * @param lambda the penalty parameter
	 */
	SubsequenceStringKernel(const std::shared_ptr<StringFeatures<char>>& l, const std::shared_ptr<StringFeatures<char>>& r,
		int32_t maxlen, float64_t lambda);

	/** destructor */
	~SubsequenceStringKernel() override;

	/**
	 * initialize kernel
	 *
	 * @param lhs features of left-hand side
	 * @param rhs features of right-hand side
	 * @return true if initialization was successful, false otherwise
	 */
	bool init(std::shared_ptr<Features> lhs, std::shared_ptr<Features> rhs) override;

	/** clean up kernel */
	void cleanup() override;

	/** @return the kernel type */
	EKernelType get_kernel_type() override
	{
		return K_POLYMATCH;
	}

	/** @return name */
	const char* get_name() const override
	{
		return "SubsequenceStringKernel";
	}

	/** register the parameters */
	void register_params() override;

	/**
	 * compute kernel function for features a and b.
	 * idx_{a,b} denote the index of the feature vectors
	 * in the corresponding feature object.
	 *
	 * The computation is achieved using two auxiliary functions \f$K'\f$ and
	 * \f$K''\f$ which are defined as follows -
	 * \f[
	 * 	K'_{i}(s,t)=\sum_{u\in\Sigma^{i}}\sum_{\mathbf{i}:u=s[\mathbf{i}]}
	 *	\sum_{\mathbf{j}:u=t[\mathbf{j}]}\lambda^{|s|+|t|-i_{1}-j_{1}+2}
	 * \f]
	 * for \f$i=1,\dots,n\f$, and
	 * \f[
	 *	K''_{i}(sx,t)=\sum_{j:t_{j}=x}K'_{i-1}(s,t[1:j-1])\lambda^{|t|-j+2}
	 * \f]
	 * where \f$K'_{0}(s,t)=0\f$ for all \f$s\f$ and \f$t\f$ (see reference for
	 * details).
	 *
	 * @param idx_a index a
	 * @param idx_b index b
	 * @return computed kernel function at indices a,b
	 */
	float64_t compute(int32_t idx_a, int32_t idx_b) override;

protected:
	/** maximum length of common subsequences */
	int32_t m_maxlen;

	/** gap penalty */
	float64_t m_lambda;
};

}
#endif // SUBSEQUENCE_STRING_KERNEL_H_
