// SPDX-License-Identifier: LGPL-2.0-or-later
// Copyright © EDF R&D / TELECOM ParisTech (ENST-TSI)

#pragma once

//Local
#include "ScalarField.h"

namespace CCCoreLib
{
	class GenericCloud;

	//! A generic class to handle a probability distribution
	/** Custom parametric distributions can be implemented through this
		interface and used for filtering data (see StatisticalTestingTools).
	**/
	class CC_CORE_LIB_API GenericDistribution
	{
	public:

		//! Default constructor
		GenericDistribution() : m_isValid(false) {}

		//! Default destructor
		virtual ~GenericDistribution() = default;

		//! Returns distribution name
		virtual const char* getName() const = 0;

		//! Indicates if the distribution parameters are valid
		/** This function is related to 'computeParameters()'. If the parameters
			computation failed, then the parameters will be marked as 'invalid'. In this
			case, the dsitribution should'nt be used (most of the methods won't work anyway).
			\return true (if the distribution parameters are valid) or false (if not)
		**/
		virtual bool isValid() const { return m_isValid; }

		//! Scalar values container interface
		struct ScalarContainer
		{
			virtual size_t size() const = 0;
			virtual ScalarType getValue(size_t index) const = 0;
		};

		//! Wrapper of a CCCoreLib's scalar field as a Scalar values container
		struct SFAsScalarContainer : ScalarContainer
		{
			SFAsScalarContainer(const ScalarField& sf)
				: ScalarContainer()
				, m_sf(sf)
			{}

			inline size_t size() const override { return m_sf.size(); }
			inline ScalarType getValue(size_t index) const override { return m_sf.getValue(index); }

			const CCCoreLib::ScalarField& m_sf;
		};

		//! Wrapper of a std::vector as a Scalar values container
		struct VectorAsScalarContainer : ScalarContainer
		{
			VectorAsScalarContainer(const std::vector<ScalarType>& vector)
				: ScalarContainer()
				, m_vector(vector)
			{}

			inline size_t size() const override { return m_vector.size(); }
			inline ScalarType getValue(size_t index) const override { return m_vector[index]; }

			const std::vector<ScalarType>& m_vector;
		};

		//! Computes the distribution parameters from a set of values
		/**	\param values a set of scalar values
			\return true (if the computation succeeded) or false (if not)
		**/
		virtual bool computeParameters(const ScalarContainer& values) = 0;

		//! Computes the probability of x
		/** \param x the variable
			\return the probabilty
		**/
		virtual double computeP(ScalarType x) const = 0;

		//! Computes the cumulative probability between 0 and x
		/** \param x the upper boundary
			\return the cumulative probabilty
		**/
		virtual double computePfromZero(ScalarType x) const = 0;

		//! Computes the cumulative probability between x1 and x2
		/** x1 should be lower than x2
			\param x1 the lower boundary
			\param x2 the upper boundary
			\return the cumulative probabilty
		**/
		virtual double computeP(ScalarType x1, ScalarType x2) const = 0;

		//! Computes the Chi2 distance (related to the Chi2 Test)
		/** Computes the Chi2 distance from a group of point, accordingly to
			a certain number of classes (see Chi2 test theory for more information).
			The result of projecting each point (or more precisely each scalar value
			associated to each point) in the different classes can be stored in an array
			(of the same size as the number of classes). To do so, such an array (already
			allocated in memory) should be passed as an argument.
			Warning: be sure to activate an OUTPUT scalar field on the input cloud
			\param Yk a group of points
			\param numberOfClasses the number of classes for the Chi2 Test
			\param histo an array to store the values projection result (optional)
			\return the Chi2 distance (or -1.0 if an error occurred)
		**/
		virtual double computeChi2Dist(const GenericCloud* Yk, unsigned numberOfClasses, int* histo = nullptr) = 0;

	protected:

		//! Sets distribution current validity
		void setValid(bool state) { m_isValid = state; }

		//! Whether the distribution is in a valid state or not
		bool m_isValid;
	};

}
